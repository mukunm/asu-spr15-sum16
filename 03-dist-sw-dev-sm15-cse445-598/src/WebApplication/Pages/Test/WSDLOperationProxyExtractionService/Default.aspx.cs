using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Reflection;

public partial class _Default : Page
{
    protected void Page_Load(object sender, EventArgs e)
    {

    }
    protected void proceedButton_Click(object sender, EventArgs e)
    {
        try
        {
            /* Create a WSDL Utils Client to access the WSDL services */
            wsdlUtilsService.WSDLServiceClient wsdlUtilsClient =
                new wsdlUtilsService.WSDLServiceClient();

            /* Extract the URL from the text box */
            string url = webPageUrlTextBox.Text;

            /* Call the service to extract the dictionary of objects which represent the operations */
            /* of the service. */
            string[] methodsInfo = wsdlUtilsClient.GetWebServiceMethodsInfo(url);

            /* If no methods were extracted, print an error message and return */
            if (methodsInfo == null || methodsInfo.Length == 0)
            {
                errorMessageLabel.Text = "Error - Couldn't extract operations from the URL";
                return;
            }

            /* Create operations tree and to the placeholder */
            TreeView operationsTree = new TreeView();
            operationsPlaceHolder.Controls.Add(operationsTree);

            /* Add a servce node to the operations tree */
            TreeNode serviceNode = new TreeNode(methodsInfo[0]);
            operationsTree.Nodes.Add(serviceNode);

            /* Traverse through the list of method information and add them */
            /* to the tree view. */
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

                    if (methodName == "Discover")
                        break;

                    TreeNode mNameNode = new TreeNode(methodName);
                    serviceNode.ChildNodes.Add(mNameNode);
                    i++;

                    int j = i;
                    while (true)
                    {
                        if (methodsInfo[j].Length > 11 &&
                            methodsInfo[j].Substring(0, 11).Equals("MethodName:"))
                            break;

                        if (methodsInfo[j].Substring(0, 6).Equals("PName:"))
                        {
                            string[] paramNameInfo =
                                methodsInfo[j].Split(new string[] { "PName:" },
                                StringSplitOptions.None);
                            string paramName = paramNameInfo[1];

                            TreeNode pNameNode = new TreeNode(paramName);
                            mNameNode.ChildNodes.Add(pNameNode);
                            j++;

                            if (methodsInfo[j].Substring(0, 6).Equals("PType:"))
                            {
                                string[] paramTypeInfo =
                                    methodsInfo[j].Split(new string[] { "PType:" },
                                    StringSplitOptions.None);
                                string paramType = paramTypeInfo[1];

                                TreeNode pTypeNode = new TreeNode(paramType);
                                pNameNode.ChildNodes.Add(pTypeNode);
                                j++;
                            }
                        }
                    }
                    i = j;
                }
            }

            /* Close the WSDL Utils Client */
            wsdlUtilsClient.Close();
        }
        catch(Exception)
        {
            errorMessageLabel.Text = "Error - Couldn't extract operations from the URL";
        }
    }
}
