using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

public partial class _Default : Page
{
    protected void Page_Load(object sender, EventArgs e)
    {

    }
    
    /* This method gets called when the user clicks the proceed button */
    protected void proceedButton_Click(object sender, EventArgs e)
    {
        parseResultPlaceHolder.Controls.Clear();
        errorMessageLabel.Text = "";

        /* Create the WSDL utils proxy */
        wsdlUtilsService.WSDLServiceClient
            wsdlUtils = new wsdlUtilsService.WSDLServiceClient();

        /* Extract the URL from the URL box */
        string url = urlTextBox.Text;
        wsdlUtilsService.MethodDetails[] methodsList;

        try
        {
            /* Call the service to extract methods and parameters */
            methodsList = wsdlUtils.ExtractWsdlOperation(url);
        }
        catch(Exception)
        {
            errorMessageLabel.Text = "Couldn't extract details. Check the link provided";
            return;
        }

        Table parseResultsTable = new Table();
        parseResultPlaceHolder.Controls.Add(parseResultsTable);

        TableHeaderRow th = new TableHeaderRow();

        Label methodNameHeaderLabel = new Label();
        methodNameHeaderLabel.Text = "Method Name";
        TableHeaderCell methodNameHeaderCell = new TableHeaderCell();
        methodNameHeaderCell.Controls.Add(methodNameHeaderLabel);
        th.Cells.Add(methodNameHeaderCell);

        Label inputParamsHeaderLabel = new Label();
        inputParamsHeaderLabel.Text = "Input Params";
        TableHeaderCell inputParamsHeaderCell = new TableHeaderCell();
        inputParamsHeaderCell.Controls.Add(inputParamsHeaderLabel);
        th.Cells.Add(inputParamsHeaderCell);

        Label outputParamsHeaderLabel = new Label();
        outputParamsHeaderLabel.Text = "Output Params";
        TableHeaderCell outputParamsHeaderCell = new TableHeaderCell();
        outputParamsHeaderCell.Controls.Add(outputParamsHeaderLabel);
        th.Cells.Add(outputParamsHeaderCell);

        parseResultsTable.Rows.Add(th);

        /* Parse through the result obtained by calling the service and create a list to be displayed */
        foreach (wsdlUtilsService.MethodDetails method in methodsList)
        {
            string ipParamString;
            string opParamString;
            string methodName = method.methodName + "<br/>";

            wsdlUtilsService.ParameterDetails[] ipParams = method.inputParameters;
            wsdlUtilsService.ParameterDetails[] opParams = method.outputParameters;

            if(ipParams.Length == 0)
            {
                ipParamString = "No input Params<br/>";
            }
            else
            {
                ipParamString = "Names: ";

                foreach (wsdlUtilsService.ParameterDetails parameter in ipParams)
                {
                    ipParamString += parameter.parameterName + ",";
                }

                ipParamString += "<br/>Types: ";

                foreach (wsdlUtilsService.ParameterDetails parameter in ipParams)
                {
                    ipParamString += parameter.parameterType + ",";
                }
                ipParamString += "<br/>";
            }

            if (opParams.Length == 0)
            {
                opParamString = "No output Params<br/>";
            }
            else
            {
                opParamString = "Names: ";

                foreach (wsdlUtilsService.ParameterDetails parameter in opParams)
                {
                    ipParamString += parameter.parameterName + ",";
                }

                opParamString += "<br/>Types: ";

                foreach (wsdlUtilsService.ParameterDetails parameter in opParams)
                {
                    opParamString += parameter.parameterType + ",";
                }
                opParamString += "<br/>";
            }
            

            TableRow tr = new TableRow();
            
            Label methodNameLabel = new Label();
            methodNameLabel.Text = methodName;
            TableCell methodNameCell = new TableCell();
            methodNameCell.Controls.Add(methodNameLabel);
            tr.Cells.Add(methodNameCell);

            Label inputParamsLabel = new Label();
            inputParamsLabel.Text = ipParamString;
            TableCell inputParamsCell = new TableCell();
            inputParamsCell.Controls.Add(inputParamsLabel);
            tr.Cells.Add(inputParamsCell);

            Label outputParamsLabel = new Label();
            outputParamsLabel.Text = opParamString;
            TableCell outputParamsCell = new TableCell();
            outputParamsCell.Controls.Add(outputParamsLabel);
            tr.Cells.Add(outputParamsCell);

            parseResultsTable.Rows.Add(tr);
        }
        wsdlUtils.Close();
    }
}