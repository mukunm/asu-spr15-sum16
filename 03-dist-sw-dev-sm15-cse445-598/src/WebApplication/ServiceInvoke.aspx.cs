using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Xml;

public partial class ServiceInvoke : Page
{
    public string currentServiceName;
    public List<wsdlUtilsService.MethodDetails> currentServiceMethodDetails;

    /* This method gets called when the page loads */
    protected void Page_Load(object sender, EventArgs e)
    {
        DataSet ds = new DataSet();
        String cmd = "UserName='" + User.Identity.Name + "'";
        FileStream fs = new FileStream(Server.MapPath("App_Data/Users.xml"),
                          FileMode.Open, FileAccess.Read);
        StreamReader reader = new StreamReader(fs);
        ds.ReadXml(reader);
        fs.Close();

        DataTable users = ds.Tables[0];
        DataRow[] matches = users.Select(cmd);

        if (matches != null && matches.Length > 0)
        {
            DataRow row = matches[0];
            String role = (String)row["Role"];
            
            if(0 == String.Compare(role, "User"))
            {
                Response.Redirect("~/Search?m=NotAnAdmin");
            }
        }

        errorMessageLabel.Text = "";

        if (!IsPostBack)
        {
            invokeButton.Visible = false;
            invokeButton.Enabled = false;
            instructionMessageLabel.Visible = false;
            invokeTypeButtonList.Visible = false;

            string serviceUrl = Request.QueryString["serviceWsdlLink"];
            if (serviceUrl != null)
            {
                serviceUrlTextBox.Text = serviceUrl;
                exploreOperationsButton_Click(exploreOperationsButton, null);
            }
        }
    }

    /* This method extracts method and parameter information provided with an */
    /* array of encoded strings representing the method and parameter info.   */
    private void extractMethodParamInfo(string[] methodsInfo)
    {
        if (currentServiceMethodDetails == null)
        {
            currentServiceMethodDetails =
                                new List<wsdlUtilsService.MethodDetails>();
        }

        /* First entry in the array is the service name */
        currentServiceName = methodsInfo[0];

        /* Traverse through the entire methods info array and add them to the */
        /* method info list global */
        int i = 1;
        while (i < methodsInfo.Length)
        {
            if (methodsInfo[i].Length > 11 &&
                methodsInfo[i].Substring(0, 11).Equals("MethodName:"))
            {
                string[] methodNameInfo =
                    methodsInfo[i].Split(new string[] { "MethodName:" },
                    StringSplitOptions.None);
                string methodName = methodNameInfo[1];

                /* If the method name is Discover, stop */
                if (methodName == "Discover")
                    break;

                wsdlUtilsService.MethodDetails methodDetails =
                                            new wsdlUtilsService.MethodDetails();

                /* Extract the method name and add to the lsit */
                methodDetails.methodName = methodName;
                List<wsdlUtilsService.ParameterDetails> inputParameterDetails =
                    new List<wsdlUtilsService.ParameterDetails>();
                List<wsdlUtilsService.ParameterDetails> outputParameterDetails =
                    new List<wsdlUtilsService.ParameterDetails>();

                /* Extract the parameters and return types and add to the list */
                int j = i + 1;
                while (true)
                {
                    if (methodsInfo[j].Length > 11 &&
                        methodsInfo[j].Substring(0, 11).Equals("MethodName:"))
                        break;

                    if (methodsInfo[j].Length > 11 &&
                        methodsInfo[j].Substring(0, 11).Equals("ReturnType:"))
                    {
                        string[] returnTypeInfo =
                            methodsInfo[j].Split(new string[] { "ReturnType:" },
                            StringSplitOptions.None);
                        string returnTypeName = returnTypeInfo[1];

                        wsdlUtilsService.ParameterDetails returnParameter =
                            new wsdlUtilsService.ParameterDetails();
                        returnParameter.parameterType = "ReturnType";
                        returnParameter.parameterName = returnTypeName;
                        outputParameterDetails.Add(returnParameter);

                        j++;
                    }

                    if (methodsInfo[j].Substring(0, 6).Equals("PName:"))
                    {
                        string[] paramNameInfo =
                            methodsInfo[j].Split(new string[] { "PName:" },
                            StringSplitOptions.None);
                        string paramName = paramNameInfo[1];

                        wsdlUtilsService.ParameterDetails inputParameter =
                            new wsdlUtilsService.ParameterDetails();
                        inputParameter.parameterName = paramName;

                        j++;

                        if (methodsInfo[j].Substring(0, 6).Equals("PType:"))
                        {
                            string[] paramTypeInfo =
                                methodsInfo[j].Split(new string[] { "PType:" },
                                StringSplitOptions.None);
                            string paramType = paramTypeInfo[1];
                            inputParameter.parameterType = paramType;

                            inputParameterDetails.Add(inputParameter);
                            j++;
                        }
                    }
                }

                methodDetails.inputParameters = inputParameterDetails.ToArray();
                methodDetails.outputParameters = outputParameterDetails.ToArray();
                currentServiceMethodDetails.Add(methodDetails);
                i = j;
            }
        }

    }

    /* This method is triggered when the explore operations button is clicked on */
    protected void exploreOperationsButton_Click(object sender, EventArgs e)
    {
        /* Create a WSDL Utils Client to access the WSDL services */
        wsdlUtilsService.WSDLServiceClient wsdlUtilsClient =
            new wsdlUtilsService.WSDLServiceClient();

        try
        {
            /* Clear previously created inputs */
            operationsLabel.Text = "";
            operationsTree.Nodes.Clear();
            parametersLabel.Text = "";
            parameterValueDataList.DataSource = null;
            parameterValueDataList.DataBind();
            invokeButton.Visible = false;
            invokeButton.Enabled = false;
            resultsLabel.Text = "";

            groupInvokeParametersLabel.Text = "";
            groupInvokeParametersInstructionLabel.Text = "";
            groupInvokeParametersInputLabel.Text = "";
            groupInvokeParametersOutputLabel.Text = "";
            groupInvokeParametersHintLabel.Text = "";
            groupInvokeList.DataSource = null;
            groupInvokeList.DataBind();
            groupInvokeResultsLabel.Text = "";
            groupInvokeResultsPanel.Controls.Clear();

            /* Extract the URL from the text box */
            string url = serviceUrlTextBox.Text;

            /* Call the service to extract the dictionary of objects which represent the operations */
            /* of the service. */
            string[] methodsInfo = wsdlUtilsClient.GetWebServiceMethodsInfo(url);

            /* If no methods were extracted, print an error message and return */
            if (methodsInfo == null || methodsInfo.Length == 0)
            {
                errorMessageLabel.Text = "Couldn't extract operations from the provided URL. Please check the link.";
                return;
            }

            /* Extract method and parameter information from the obtained array */
            extractMethodParamInfo(methodsInfo);

            /* Add a servce node to the operations tree */
            TreeNode serviceNode = new TreeNode(currentServiceName);
            serviceNode.SelectAction = TreeNodeSelectAction.Select;
            operationsTree.Nodes.Add(serviceNode);

            wsdlUtilsService.MethodDetails[] methodDetails =
                                            currentServiceMethodDetails.ToArray();

            /* Traverse through the method details list and add them to the operations tree */
            for (int i = 0; i < methodDetails.Length; i++)
            {
                TreeNode methodNode = new TreeNode(methodDetails[i].methodName);
                methodNode.SelectAction = TreeNodeSelectAction.Select;
                serviceNode.ChildNodes.Add(methodNode);
            }

            operationsLabel.Text = "Operations Available";
            operationsTree.ExpandAll();

            instructionMessageLabel.Visible = true;
            invokeTypeButtonList.Visible = true;

            /* Close the WSDL Utils Client */
            wsdlUtilsClient.Close();
        }
        catch (Exception ec)
        {
            errorMessageLabel.Text = "Couldn't extract operations from the provided URL. Please check the link.";
            wsdlUtilsClient.Close();
        }
    }

    /* This method gets triggered when a node is selected in the operations tree */
    protected void operationsTree_SelectedNodeChanged(object sender, EventArgs e)
    {
        if (invokeTypeButtonList.SelectedItem == null)
        {
            errorMessageLabel.Text = "You need to select an invocation type to proceed.";
            return;
        }

        /* Create a WSDL Utils Client to access the WSDL services */
        wsdlUtilsService.WSDLServiceClient wsdlUtilsClient =
            new wsdlUtilsService.WSDLServiceClient();

        if (invokeTypeButtonList.SelectedItem.Value == "SingleInvoke")
        {
            try
            {
                groupInvokeParametersLabel.Text = "";
                groupInvokeParametersInstructionLabel.Text = "";
                groupInvokeParametersInputLabel.Text = "";
                groupInvokeParametersOutputLabel.Text = "";
                groupInvokeParametersHintLabel.Text = "";
                groupInvokeList.DataSource = null;
                groupInvokeList.DataBind();
                groupInvokeResultsLabel.Text = "";
                groupInvokeResultsPanel.Controls.Clear();
                resultsLabel.Text = "";
                resultsPanel.Controls.Clear();

                /* Get the selected operation */
                string selectedOperationName = operationsTree.SelectedNode.Text;
                operationsTree.SelectedNode.Selected = false;

                /* Extract the URL from the text box */
                string url = serviceUrlTextBox.Text;

                /* Call the service to extract the dictionary of objects which represent the operations */
                /* of the service. */
                string[] methodsInfo = wsdlUtilsClient.GetWebServiceMethodsInfo(url);

                wsdlUtilsClient.Close();

                /* If no methods were extracted, print an error message and return */
                if (methodsInfo == null || methodsInfo.Length == 0)
                {
                    errorMessageLabel.Text = "Couldn't extract operations from the provided URL. Please check the link.";
                    return;
                }

                /* Extract method and parameter information from the obtained array */
                extractMethodParamInfo(methodsInfo);

                /* Get parameter information for the selected operation */
                wsdlUtilsService.MethodDetails[] methodDetails = currentServiceMethodDetails.ToArray();

                if (methodDetails == null)
                {
                    errorMessageLabel.Text = "Couldn't extract operations from the provided URL. Please check the link.";
                    return;
                }

                int i;
                for (i = 0; i < methodDetails.Length; i++)
                {
                    if (methodDetails[i].methodName == selectedOperationName)
                        break;
                }

                if (i == methodDetails.Length)
                {
                    errorMessageLabel.Text = "Couldn't find the requested operation";
                    return;
                }

                wsdlUtilsService.ParameterDetails[] parameters = methodDetails[i].inputParameters;

                /* Built in types to look for */
                string[] builtInTypes = new string[] { "System.Boolean", "System.Char", "System.Decimal", 
                                                       "System.Double", "System.Single", "System.Int32", 
                                                       "System.UInt32", "System.Int64", "System.UInt64", 
                                                       "System.Int16", "System.UInt16", "System.String" };

                /* Check if any of the parameters have a type that is not a built-in type */
                for (i = 0; i < parameters.Length; i++)
                {
                    int j;
                    for (j = 0; j < builtInTypes.Length; j++)
                    {
                        if (parameters[i].parameterType == builtInTypes[j])
                            break;
                    }

                    if (j == builtInTypes.Length)
                    {
                        /* Method contains an input parameter of type that is not built-in, return */
                        /* Display error message and return */
                        errorMessageLabel.Text = "The system doesn't support invocation of operations with complex input parameters";
                        return;
                    }
                }

                List<wsdlUtilsService.ParameterDetails> parameterList = new List<wsdlUtilsService.ParameterDetails>();
                for (i = 0; i < parameters.Length; i++)
                    parameterList.Add(parameters[i]);

                parameterValueDataList.DataSource = parameterList;
                parameterValueDataList.DataBind();

                parametersLabel.Text = selectedOperationName;
                invokeButton.Visible = true;
                invokeButton.Enabled = true;
            }
            catch (Exception ec)
            {
                errorMessageLabel.Text = "Couldn't extract operations from the provided URL. Please check the link.";
                wsdlUtilsClient.Close();
            }
        }
        else
        {
            try
            {
                operationsTree.SelectedNode.Selected = false;
                parametersLabel.Text = "";
                parameterValueDataList.DataSource = null;
                parameterValueDataList.DataBind();
                resultsLabel.Text = "";
                resultsPanel.Controls.Clear();

                /* Extract the URL from the text box */
                string url = serviceUrlTextBox.Text;

                /* Call the service to extract the dictionary of objects which represent the operations */
                /* of the service. */
                string[] methodsInfo = wsdlUtilsClient.GetWebServiceMethodsInfo(url);

                wsdlUtilsClient.Close();

                /* If no methods were extracted, print an error message and return */
                if (methodsInfo == null || methodsInfo.Length == 0)
                {
                    errorMessageLabel.Text = "Couldn't extract operations from the provided URL. Please check the link.";
                    return;
                }

                /* Extract method and parameter information from the obtained array */
                extractMethodParamInfo(methodsInfo);

                /* Get parameter information for the selected operation */
                wsdlUtilsService.MethodDetails[] methodDetails = currentServiceMethodDetails.ToArray();

                if (methodDetails == null)
                {
                    errorMessageLabel.Text = "Couldn't extract operations from the provided URL. Please check the link.";
                    return;
                }

                int i;


                /* Built in types to look for */
                string[] builtInTypes = new string[] { "System.Boolean", "System.Char", "System.Decimal", 
                                                       "System.Double", "System.Single", "System.Int32", 
                                                       "System.UInt32", "System.Int64", "System.UInt64", 
                                                       "System.Int16", "System.UInt16", "System.String" };

                List<wsdlUtilsService.ParameterDetails> parameterList = new List<wsdlUtilsService.ParameterDetails>();

                /* Check if any of the parameters have a type that is not a built-in type */
                for (int k = 0; k < methodDetails.Length; k++)
                {
                    wsdlUtilsService.ParameterDetails[] parameters = methodDetails[k].inputParameters;

                    for (i = 0; i < parameters.Length; i++)
                    {
                        int j;
                        for (j = 0; j < builtInTypes.Length; j++)
                        {
                            if (parameters[i].parameterType == builtInTypes[j])
                                break;
                        }

                        if (j == builtInTypes.Length)
                        {
                            /* Method contains an input parameter of type that is not built-in, return */
                            /* Display error message and return */
                            errorMessageLabel.Text = "The system doesn't support invocation of operations with complex input parameters.<br/>" +
                                                     " One of the operations requires complex input parameters. Please try Single Invoke.";
                            return;
                        }

                        parameterList.Add(parameters[i]);
                    }
                }

                groupInvokeList.DataSource = methodDetails;
                groupInvokeList.DataBind();

                groupInvokeParametersLabel.Text = currentServiceName + " - Group Invoke Test Service ";
                groupInvokeParametersInstructionLabel.Text =
                                                  "Enter inputs and outputs to each and every input parameters and return " +
                                                  "types of each and every operation for the service chosen.";
                groupInvokeParametersInputLabel.Text =
                                                  "Input parameter format - Needs to be as expected by the operation chosen.";
                groupInvokeParametersOutputLabel.Text =
                                                  "Output parameter format - Needs to be in XML format";
                groupInvokeParametersHintLabel.Text =
                                                  "Hint - To enter sample inputs and outputs, one can use the single invoke" +
                                                  "operation before proceeding to group invoke";
                invokeButton.Visible = true;
                invokeButton.Enabled = true;
            }
            catch (Exception ec)
            {
                errorMessageLabel.Text = "Couldn't extract operations from the provided URL. Please check the link.";
                wsdlUtilsClient.Close();
            }
        }
    }

    /* This method gets triggered when the groupInvoke list gets bound with data */
    protected void groupInvokeList_ItemDataBound(object sender, DataListItemEventArgs e)
    {
        if (e.Item.ItemType == ListItemType.Item || e.Item.ItemType == ListItemType.AlternatingItem)
        {
            wsdlUtilsService.MethodDetails method = (wsdlUtilsService.MethodDetails)e.Item.DataItem;
            
            DataList inputParameterDataList = (DataList)e.Item.FindControl("groupInvokeInputParameterList");
            DataList outputParameterDataList = (DataList)e.Item.FindControl("groupInvokeOutputParameterList");

            inputParameterDataList.DataSource = method.inputParameters;
            inputParameterDataList.DataBind();

            outputParameterDataList.DataSource = method.outputParameters;
            outputParameterDataList.DataBind();
        }
    }

    /* This method gets triggered when invoke button is clicked on */
    protected void invokeButton_Click(object sender, EventArgs e)
    {
        if (invokeTypeButtonList.SelectedItem == null)
        {
            errorMessageLabel.Text = "You need to select an invocation type to proceed.";
            return;
        }

        if (invokeTypeButtonList.SelectedItem.Value == "SingleInvoke")
        {
            groupInvokeParametersLabel.Text = "";
            groupInvokeParametersInstructionLabel.Text = "";
            groupInvokeParametersInputLabel.Text = "";
            groupInvokeParametersOutputLabel.Text = "";
            groupInvokeParametersHintLabel.Text = "";
            groupInvokeList.DataSource = null;
            groupInvokeList.DataBind();
            groupInvokeResultsLabel.Text = "";
            groupInvokeResultsPanel.Controls.Clear();

            List<string> parameters = new List<string>();

            /* Traverse through all the parameterValues and add them to a list */
            foreach (DataListItem item in parameterValueDataList.Items)
            {
                string parameterValue = ((TextBox)item.FindControl("parameterValue")).Text;

                /* If you find that the parameter value is empty, return */
                if (parameterValue == "")
                {
                    errorMessageLabel.Text = "You need to enter all the input parameters to invoke an operation";
                    return;
                }

                parameters.Add(parameterValue);
            }

            string url = serviceUrlTextBox.Text;

            wsdlUtilsService.WSDLServiceClient wsdlUtilsClient =
                new wsdlUtilsService.WSDLServiceClient();

            try
            {
                /* Invoke the web service method */
                wsdlUtilsService.ReturnData returnData =
                        wsdlUtilsClient.InvokeWebServiceMethod(url, parametersLabel.Text,
                                                               parameters.ToArray());

                /* If return data is null, return */
                if (returnData == null)
                {
                    errorMessageLabel.Text = "The invocation didn't yield any result.";
                    return;
                }

                /* Display the result */
                TextBox resultXmlTextBox = new TextBox();
                resultXmlTextBox.Text = returnData.objectXML;
                resultXmlTextBox.Rows = 15;
                resultXmlTextBox.Columns = 70;
                resultXmlTextBox.TextMode = TextBoxMode.MultiLine;

                resultsLabel.Text = "Results";
                resultsPanel.Controls.Add(resultXmlTextBox);
            }
            catch (Exception ec)
            {
                errorMessageLabel.Text = "Invocation didn't succeed. Try again later.";
            }
        }
        else
        {
            parametersLabel.Text = "";
            parameterValueDataList.DataSource = null;
            parameterValueDataList.DataBind();
            resultsLabel.Text = "";
            resultsPanel.Controls.Clear();

            List<string> inputParameters = new List<string>();
            List<string> outputParameters = new List<string>();

            foreach (DataListItem item in groupInvokeList.Items)
            {
                DataList inputParameterDataList = (DataList)item.FindControl("groupInvokeInputParameterList");
                DataList outputParameterDataList = (DataList)item.FindControl("groupInvokeOutputParameterList");

                /* Traverse through all the parameterValues and add them to a list */
                foreach (DataListItem inputItem in inputParameterDataList.Items)
                {
                    TextBox textBox = (TextBox)inputItem.FindControl("groupInvokeInputParameterValue");
                    string inputParameterValue = ((TextBox)inputItem.FindControl("groupInvokeInputParameterValue")).Text;

                    /* If you find that the parameter value is empty, return */
                    if (inputParameterValue == "")
                    {
                        errorMessageLabel.Text = "You need to enter all the input parameters to invoke an operation";
                        return;
                    }

                    inputParameters.Add(inputParameterValue);
                }

                /* Traverse through all the parameterValues and add them to a list */
                foreach (DataListItem outputItem in outputParameterDataList.Items)
                {
                    TextBox textBox = (TextBox)outputItem.FindControl("groupInvokeOutputParameterValue");
                    string outputParameterValue = ((TextBox)outputItem.FindControl("groupInvokeOutputParameterValue")).Text;

                    /* If you find that the parameter value is empty, return */
                    if (outputParameterValue == "")
                    {
                        errorMessageLabel.Text = "You need to enter all the output parameters to invoke an operation";
                        return;
                    }

                    outputParameters.Add(outputParameterValue);
                }
            }

            string url = serviceUrlTextBox.Text;

            wsdlUtilsService.WSDLServiceClient wsdlUtilsClient =
                new wsdlUtilsService.WSDLServiceClient();

            try
            {
                /* Invoke the web service method */
                wsdlUtilsService.InvokeResult invokeResult =
                        wsdlUtilsClient.InvokeWebServiceOperationsGroup(url, inputParameters.ToArray(),
                                                                        outputParameters.ToArray());

                /* If return data is null, return */
                if (invokeResult == null)
                {
                    errorMessageLabel.Text = "The invocation didn't yield any result.";
                    return;
                }

                Table resultsTable = new Table();
                TableHeaderRow resultsHeader = new TableHeaderRow();
                TableHeaderCell methodHeader = new TableHeaderCell();
                methodHeader.Text = "Method Name";
                TableHeaderCell statusHeader = new TableHeaderCell();
                statusHeader.Text = "Invoke Status";
                resultsHeader.Cells.Add(methodHeader);
                resultsHeader.Cells.Add(statusHeader);
                resultsTable.Rows.Add(resultsHeader);

                for (int i = 0; i < invokeResult.MethodName.Length; i++)
                {
                    TableRow row = new TableRow();
                    TableCell methodCell = new TableCell();
                    TableCell statusCell = new TableCell();
                    methodCell.Text = invokeResult.MethodName[i];

                    if (invokeResult.InvokeStatus[i] == true)
                        statusCell.Text = "Success";
                    else
                        statusCell.Text = "Failure";

                    row.Cells.Add(methodCell);
                    row.Cells.Add(statusCell);
                    resultsTable.Rows.Add(row);
                }

                groupInvokeResultsPanel.Controls.Add(resultsTable);
                groupInvokeResultsLabel.Text = "Results";
            }
            catch (Exception ec)
            {
                errorMessageLabel.Text = "Invocation didn't succeed. Try again later.";
            }

        }
    }

}