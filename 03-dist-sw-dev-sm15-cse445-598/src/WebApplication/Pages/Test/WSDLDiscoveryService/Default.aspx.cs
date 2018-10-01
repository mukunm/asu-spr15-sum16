using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Xml;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;


public partial class _Default : Page
{
    public static string[] urlInfos = new string[50];
    public static int count = 0;


    protected void Page_Load(object sender, EventArgs e)
    {

    }

    public static void parseResponseXML(XmlNode node)
    {
        /* Return in case the pre requisites for parsing are null */
        if (urlInfos == null || node == null)
            return;

        /* Obtain the XML node value and add it to the URL info array */
        if (node.Name == "#text")
            urlInfos[count++] = node.Value;

        /* Parse the XML DOM recursively if there are child nodes */
        if (node.HasChildNodes)
        {
            XmlNodeList children = node.ChildNodes;
            foreach (XmlNode child in children)
                parseResponseXML(child);
        }
    }
    
    /* This method gets called when the user clicks the search button */
    protected void searchButton_Click(object sender, EventArgs e)
    {
        try
        {
            /* Clear previously rendered results */
            errorMessageLabel.Text = "";
            searchResultsPlaceHolder.Controls.Clear();

            /* Extract keywords from the keyword text box */
            string keyword = searchTextBox.Text;

            /* Create the URL string */
            string url = "http://webstrar45.fulton.asu.edu/Page0/WSDLSearchService.svc/DiscoverWsdlServices?keywords=" + keyword;

            /* Create a HTTP web request and extract the response */
            HttpWebRequest request = WebRequest.Create(url) as HttpWebRequest;
            HttpWebResponse response = request.GetResponse() as HttpWebResponse;

            /* Load the response stream into an XML DOM object */
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(response.GetResponseStream());
            XmlNode nodeRef = xmlDoc.DocumentElement;

            /* Parse the XML DOM to obtain all the URL Infos */
            parseResponseXML(nodeRef);

            Table searchResultsTable = new Table();
            searchResultsPlaceHolder.Controls.Add(searchResultsTable);

            TableHeaderRow th = new TableHeaderRow();
        
            Label titleLabel = new Label();
            titleLabel.Text = "Title";
            TableHeaderCell titleCell = new TableHeaderCell();
            titleCell.Controls.Add(titleLabel);
            th.Cells.Add(titleCell);

            Label linkLabel = new Label();
            linkLabel.Text = "Link";
            TableHeaderCell linkCell = new TableHeaderCell();
            linkCell.Controls.Add(linkLabel);
            th.Cells.Add(linkCell);

            searchResultsTable.Controls.Add(th);

            /* Traverse through the entire list of URLs obtained, and create a list */
            /* of URLs and their titles.                                            */
            foreach(var urlInfo in urlInfos)
            {
                if (urlInfo == null || urlInfo == "")
                    break;

                TableRow tr = new TableRow();
            
                string[] urlInfoDecoded = urlInfo.Split(new String[] {"-[TL-LNK-SEP]-"}, 
                                                        StringSplitOptions.None);
                Label titleValue = new Label();
                titleValue.Text = urlInfoDecoded[0];
                TableCell titleValueCell = new TableCell();
                titleValueCell.Controls.Add(titleValue);
                tr.Cells.Add(titleValueCell);

                HyperLink hyperlink = new HyperLink();
                hyperlink.Text = urlInfoDecoded[1];
                hyperlink.NavigateUrl = urlInfoDecoded[1];
                hyperlink.Target = urlInfoDecoded[1];
                TableCell linkValueCell = new TableCell();
                linkValueCell.Controls.Add(hyperlink);
                tr.Cells.Add(linkValueCell);

                searchResultsTable.Controls.Add(tr);
            }
        }
        catch (Exception)
        {
            /* Display an error message in case of an exception */
            errorMessageLabel.Text = "Error - Reached Google Search API Max limit";
            return;
        }
    }
}