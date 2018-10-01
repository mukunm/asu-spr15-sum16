using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web;
using System.Web.Http;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Xml;

public partial class Search : Page
{
    public static string[] urlInfos;
    public static int count;

    public struct UrlInfo
    {
        public string title 
        {
            get;
            set;
        }

        public string link
        {
            get;
            set;
        }

        public string shortenedLink
        {
            get;
            set;
        }

        public string methodDetails
        {
            get;
            set;
        }
    }

    protected void Page_Load(object sender, EventArgs e)
    {
        String queryString = Request.QueryString["m"];
        
        if(!String.IsNullOrEmpty(queryString))
        {
            errorMessageLabel.Text = "Since you're not an Admin user, you cannot access Service Invoke directly.";
        }
    }

    /* Parse XML response. */
    public static void parseResponseXML(XmlNode node)
    {
        /* Return in case the pre requisites for parsing are null */
        if (node == null)
            return;

        /* Create URL Info array */
        if (urlInfos == null)
        {
            urlInfos = new string[50];
            count = 0;
        }

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

    /* This method gets triggered when search button gets clicked */
    protected void searchButton_Click(object sender, EventArgs e)
    {
        wsdlUtilsService.WSDLServiceClient wsdlUtilsClient =
            new wsdlUtilsService.WSDLServiceClient();
        try
        {
            /* Clear previously rendered results */
            errorMessageLabel.Text = "";
            searchResultsList.DataSource = null;
            searchResultsList.DataBind();

            /* Extract keywords from the keyword text box */
            string keyword = searchKeywordTextBox.Text;

            /* Create the URL string */
            string url = "http://webstrar45.fulton.asu.edu/Page0/WSDLSearchService.svc/DiscoverWsdlServices?keywords=" + keyword;
            //string url = "http://localhost:52677/WSDLSearchService.svc/DiscoverWsdlServices?keywords=" + keyword;

            /* Create a HTTP web request and extract the response */
            HttpWebRequest request = WebRequest.Create(url) as HttpWebRequest;
            HttpWebResponse response;

            response = request.GetResponse() as HttpWebResponse;

            /* Load the response stream into an XML DOM object */
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(response.GetResponseStream());
            XmlNode nodeRef = xmlDoc.DocumentElement;

            /* Parse the XML DOM to obtain all the URL Infos */
            parseResponseXML(nodeRef);

            List<UrlInfo> urlInfoStructures = new List<UrlInfo>();

            int p = 0;

            /* Traverse through the entire list of URLs obtained, and create a list */
            /* of URLs and their titles.                                            */
            foreach (var urlInfo in urlInfos)
            {
                p++;
                if (urlInfo == null || urlInfo == "")
                    break;

                string[] urlInfoDecoded = urlInfo.Split(new String[] { "-[TL-LNK-SEP]-" },
                                                        StringSplitOptions.None);

                if (urlInfoDecoded[1].Contains(".pdf") ||
                    urlInfoDecoded[1].Contains(".PDF"))
                    continue;

                /* If link is a WSDL link, add it to the table. If the link is not a WSDL link */
                /* look for WSDL addresses in the link. */
                if (urlInfoDecoded[1].EndsWith(".wsdl", StringComparison.OrdinalIgnoreCase) ||
                    urlInfoDecoded[1].EndsWith(".svc?wsdl", StringComparison.OrdinalIgnoreCase) ||
                    urlInfoDecoded[1].EndsWith(".asmx?wsdl", StringComparison.OrdinalIgnoreCase) ||
                    urlInfoDecoded[1].EndsWith(".php?wsdl", StringComparison.OrdinalIgnoreCase))
                {
                    UrlInfo urlInfoStructure = new UrlInfo();

                    if (urlInfoDecoded[0].Length > 20)
                        urlInfoDecoded[0] = urlInfoDecoded[0].Substring(0, 20) + " ...";

                    if (urlInfoDecoded[0] == "")
                        urlInfoDecoded[0] = "No-Title";

                    urlInfoStructure.title = urlInfoDecoded[0];
                    urlInfoStructure.link = urlInfoDecoded[1];
                    urlInfoStructure.methodDetails = "";

                    try
                    {
                        wsdlUtilsService.MethodDetails[] methodDetails =
                            wsdlUtilsClient.ExtractWsdlOperation(urlInfoDecoded[1]);

                        for (int j = 0; j < methodDetails.Length; j++)
                        {
                            if (urlInfoStructure.methodDetails.Length <= 50)
                                urlInfoStructure.methodDetails += methodDetails[j].methodName + ", ";
                        }
                    }
                    catch (Exception)
                    {
                        // Do nothing
                    }

                    urlInfoStructure.methodDetails += "...";

                    if (urlInfoDecoded[1].Length > 25)
                        urlInfoDecoded[1] = urlInfoDecoded[1].Substring(0, 25) + " ...";

                    urlInfoStructure.shortenedLink = urlInfoDecoded[1];
                    urlInfoStructures.Add(urlInfoStructure);
                }
                /* If not, extract all the addresses from the link. */
                else
                {
                    try
                    {
                        string[] extractedAddresses =
                            wsdlUtilsClient.ExtractWsdlAddresses(urlInfoDecoded[1]);

                        for (int i = 0; i < extractedAddresses.Length; i++)
                        {
                            UrlInfo urlInfoStructure = new UrlInfo();

                            urlInfoStructure.title = "No-Title";
                            urlInfoStructure.link = extractedAddresses[i];
                            urlInfoStructure.methodDetails = "";

                            try
                            {
                                wsdlUtilsService.MethodDetails[] methodDetails =
                                    wsdlUtilsClient.ExtractWsdlOperation(extractedAddresses[i]);

                                for (int j = 0; j < methodDetails.Length; j++)
                                {
                                    if (urlInfoStructure.methodDetails.Length <= 50)
                                        urlInfoStructure.methodDetails += methodDetails[j].methodName + ", ";
                                }
                            }
                            catch (Exception)
                            {
                                // Do nothing
                            }

                            urlInfoStructure.methodDetails += "...";

                            if (extractedAddresses[i].Length > 25)
                                extractedAddresses[i] = extractedAddresses[i].Substring(0, 25) + " ...";

                            urlInfoStructure.shortenedLink = extractedAddresses[i];

                            urlInfoStructures.Add(urlInfoStructure);
                        }
                    }
                    catch(Exception)
                    {
                        // Do nothing
                    }
                }
            }
            
            wsdlUtilsClient.Close();

            searchResultsList.DataSource = urlInfoStructures.ToArray();
            searchResultsList.DataBind();
            urlInfos = null;
            count = 0;
        }
        catch (Exception)
        {
            /* Display an error message in case of an exception */
            errorMessageLabel.Text = "Error - Reached Google Search API Max limit";
            urlInfos = null;
            count = 0;

            List<UrlInfo> urlInfoStructures = new List<UrlInfo>();
            UrlInfo urlInfo1 = new UrlInfo();
            urlInfo1.title = "Title 1";
            urlInfo1.link = "http://api.bing.net/search.wsdl";
            urlInfo1.shortenedLink = "http://api.bing.net/...";
            urlInfoStructures.Add(urlInfo1);
            UrlInfo urlInfo2 = new UrlInfo();
            urlInfo2.title = "Title 2";
            urlInfo2.link = "http://www.webservicex.com/globalweather.asmx?WSDL";
            urlInfo2.shortenedLink = "http://www.webservic..";
            urlInfoStructures.Add(urlInfo2);
            searchResultsList.DataSource = urlInfoStructures.ToArray();
            searchResultsList.DataBind();

            wsdlUtilsClient.Close();
            return;
        }
    }
}