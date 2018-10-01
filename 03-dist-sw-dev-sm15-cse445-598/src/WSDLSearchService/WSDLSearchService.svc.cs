using Google.Apis.Customsearch.v1;
using Google.Apis.Customsearch.v1.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;

namespace WSDLSearchService
{
    public class WSDLSearchService : IWSDLSearchService
    {
        /* API key is created using Google's Developer Console */
        public static string GOOGLE_API_KEY = "AIzaSyCYKQFm6kjnb0fA7TDGu5p6Tml513YgpGk";

        /* The search engine is created using Google's Custom Search engine creation control panel */
        public static string GOOGLE_SEARCH_ENGINE_ID = "011747664184491915424:tet0ydvuzws";
        //public static string GOOGLE_SEARCH_ENGINE_ID = "011747664184491915424:rtrrzycjpzu";

        /* The string that separates the tile and link in the returned search result */
        public static string URL_TITLE_LINK_SEPARATOR = "-[TL-LNK-SEP]-";

        /* Maximum search count for each call has to be limited because */
        /* google allows only 100 search requests per day.              */
        public static int MAX_SEARCH_COUNT = 5;

        /* Google limits the number of pages resulting from each search */
        /* to 10. If more results need to be extracted, the search needs*/
        /* to be performed again with the start index set to the next   */
        /* index.                                                       */
        public static int MAX_PAGES_PER_SEARCH = 10;

        public string GetData(int value)
        {
            return string.Format("You entered: {0}", value);
        }

        /* This service method calls the google API to discover WSDL services */
        public string[] DiscoverWsdlServices(string keywords)
        {
            int i = 0;

            /* The keyword WSDL is appended to the provided search query */
            /* to ensure that returned results are more in-line with     */
            /* the requirement, i.e. WSDL services.                      */
            string query = keywords + " WSDL";

            /* Create an array of URLs to be returned */
            string[] urls = new string[MAX_SEARCH_COUNT * MAX_PAGES_PER_SEARCH];

            /* Create a custom search service object and set the API key */
            CustomsearchService customSearchService =
                    new CustomsearchService(
                        new Google.Apis.Services.BaseClientService.Initializer()
                        {
                            ApiKey = GOOGLE_API_KEY
                        });

            /* Create a list request object to perform the search */
            CseResource.ListRequest listRequest = customSearchService.Cse.List(query);

            /* Set the search engine ID */
            listRequest.Cx = GOOGLE_SEARCH_ENGINE_ID;

            /* This loop varies the start index and repeatedly performs the search */
            for (int k = 1;
                     k < (MAX_SEARCH_COUNT * MAX_PAGES_PER_SEARCH);
                     k += MAX_PAGES_PER_SEARCH)
            {
                /* Setting the start index */
                listRequest.Start = k;

                /* Perform the search. Along with the query to search for, the WSDL search */
                /* would need certain URL patterns to look for. These URL patterns such as */
                /* "*.wsdl", "*.svc?wsdl", "*.asmx?wsdl" and "*.php?wsdl" are programmed   */
                /* into the search engine while creating it using the google's custom      */
                /* search engine control panel. The search engine ID helps recognize the   */
                /* custom search engine that has been created.                             */
                Search search = listRequest.Execute();

                /* If the search returns no results, break. There's no */
                /* point in searching more.                            */
                if (search.Items == null)
                    continue;

                /* Traverse the search result list and copy links that match WSDL type URLs */
                /* into an array of strings to be returned                                  */
                foreach (var item in search.Items)
                {
#if FILTER
                    if (item.Link.EndsWith(".wsdl") ||
                        item.Link.EndsWith(".svc?wsdl") ||
                        item.Link.EndsWith(".asmx?wsdl") ||
                        item.Link.EndsWith(".php?wsdl"))
#endif 
                    {
                        urls[i++] = item.Title +
                                    URL_TITLE_LINK_SEPARATOR +
                                    item.Link;
                    }
                }
            }

            return urls;
        }
    }
}
