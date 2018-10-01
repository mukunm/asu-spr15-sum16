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
    
    protected void proceedButton_Click(object sender, EventArgs e)
    {
        addressResultsPlaceHolder.Controls.Clear();
        errorMessageLabel.Text = "";

        /* Create a WSDL utilities client */
        wsdlUtilsService.WSDLServiceClient wsdlUtilsClient =
            new wsdlUtilsService.WSDLServiceClient();

        /* Create an array to store all the URLs */
        string[] urls;

        try
        {
            /* Extract the URLs by calling the WSDL Utilities service */
            urls = wsdlUtilsClient.ExtractWsdlAddresses(webPageUrlTextBox.Text);
        }
        catch (Exception)
        {
            errorMessageLabel.Text = "Could not find any WSDL URLs";
            return;
        }

        /* If no URLs were found, return an error message */
        if (urls == null || urls.Length == 0)
        {
            errorMessageLabel.Text = "Could not find any WSDL URLs";
            return;
        }

        /* Create a Table to store the URL results */
        Table urlResultsTable = new Table();
        addressResultsPlaceHolder.Controls.Add(urlResultsTable);

        /* Create the Header row and the appropriate label to it */
        TableHeaderRow th = new TableHeaderRow();
        Label linkHeaderLabel = new Label();
        linkHeaderLabel.Text = "Link";
        TableHeaderCell linkHeaderCell = new TableHeaderCell();
        linkHeaderCell.Controls.Add(linkHeaderLabel);
        th.Cells.Add(linkHeaderCell);

        urlResultsTable.Rows.Add(th);

        /* Parse through the list of URLs and add one row per result to the table */
        for (int i = 0; i < urls.Length; i++)
        {
            /* Break, if you find a Null pointer */
            if (urls[i] == null)
                break;

            /* Add a new row for each URL */
            TableRow tr = new TableRow();

            HyperLink link = new HyperLink();
            link.Text = urls[i];
            link.NavigateUrl = urls[i];
            TableCell linkCell = new TableCell();
            linkCell.Controls.Add(link);
            tr.Cells.Add(linkCell);

            urlResultsTable.Rows.Add(tr);
        }

        /* Close the WSDL Utils client */
        wsdlUtilsClient.Close();
    }
}