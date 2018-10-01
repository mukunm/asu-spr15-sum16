<%@ Page Title="SOAP Explore - Test Pages" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeFile="Test.aspx.cs" Inherits="Test" %>

<asp:Content ID="BodyContent" ContentPlaceHolderID="MainContent" runat="server">
    <div class="jumbotron" style="width:725px;">
    <a href="http://mukunm.github.io/" target="_blank">
        <img src="MukundManikarnike.png" alt="Mukund Manikarnike" width="600" height="60"/>
    </a>
    </div>

    <div class="row">
        <div class="col-md-12">
            <table border="0">
                <tr>
                    <th>
                        Service Prototype
                    </th>
                    <th>
                        Service Description
                    </th>
                    <th>
                        Resources Required
                    </th>
                    <th>
                        Link
                    </th>
                </tr>
                <tr>
                    <td colspan="4" style="height:15px;"></td>
                </tr>
                <tr>
                    <td colspan="4"><b><u>Developed For Assignment-3 (Part-1)</u></b></td>
                </tr>
                <tr></tr>
                <tr>
                    <td>
                        <br /><i>WSDL Discovery Service</i><br/>
                        Input: String (Keyword)<br/>
                        Output: List of URLs<br/>
                    </td>
                    <td>
                        This service will search for services <br/>
                        provided with a keyword.
                    </td>
                    <td>
                        This uses the Google Custom Search Engine Api.
                    </td>
                    <td>
                        <asp:hyperlink 
                            runat="server" 
                            NavigateUrl="~/Pages/Test/WSDLDiscoveryService/Default.aspx" 
                            Target="_blank"
                            Text="Try It">
                        </asp:hyperlink>
                    </td>
                </tr>
                <tr></tr>
                <tr>
                    <td>
                        <br /><i>WSDL Address Extraction Service</i><br/>
                        Input: String (URL)<br/>
                        Output: List of WSDL URLs<br/>
                    </td>
                    <td>
                        Given a URL, this service extracts all the URLs<br/>
                        within the webpage which contain a link to a WSDL<br/>
                        service.
                    </td>
                    <td>
                        All libraries required are present within .Net and<br/>
                        no external services are required.
                    </td>
                    <td>
                        <asp:hyperlink 
                            runat="server" 
                            NavigateUrl="~/Pages/Test/WSDLAddressExtractionService/Default.aspx" 
                            Target="_blank"
                            Text="Try It">
                        </asp:hyperlink>
                    </td>
                </tr>
                <tr>
                    <td colspan="4" style="height:15px;"></td>
                </tr>
                <tr>
                    <td colspan="4"><b><u>Developed For Assignment-3 (Part-2)</u></b></td>
                </tr>
                <tr></tr>
                <tr>
                    <td>
                        <br /><i>WSDL Operation Extraction Service</i><br/>
                        Input: String (URL)<br/>
                        Output: List of Methods and Operations<br/>
                    </td>
                    <td>
                        Given a WSDL Service URL, this service extracts<br/>
                        all the parameters and input types for the WSDL<br/>
                        service.
                    </td>
                    <td>
                        All libraries required are present within .Net and<br/>
                        no external services are required.
                    </td>
                    <td>
                        <asp:hyperlink 
                            runat="server" 
                            NavigateUrl="~/Pages/Test/WSDLOperationExtractionService/Default.aspx" 
                            Target="_blank"
                            Text="Try It">
                        </asp:hyperlink>
                    </td>
                </tr>
                <tr></tr>
                <tr>
                    <td>
                        <br /><i>WSDL Operation Proxy Extraction Service</i><br/>
                        Input: String (URL)<br/>
                        Output: Table of Service and MethodInfo objects<br/>
                    </td>
                    <td>
                        Given a WSDL URL, this service will return method<br/>
                        and operation objects which can be used for execution.
                    </td>
                    <td>
                        All libraries required are present within .Net and<br/>
                        no external services are required.
                    </td>
                    <td>
                        <asp:hyperlink 
                            runat="server" 
                            NavigateUrl="~/Pages/Test/WSDLOperationProxyExtractionService/Default.aspx" 
                            Target="_blank"
                            Text="Try It">
                        </asp:hyperlink>
                    </td>
                </tr>
                <tr>
                    <td colspan="4" style="height:15px;"></td>
                </tr>
                <tr>
                    <td colspan="4"><b><u>Developed For Assignment-3 (Graduate Project)</u></b></td>
                </tr>
                <tr></tr>
                <tr>
                    <td>
                        <br /><i>WSDL Service Testing Service - Single Invoke</i><br/>
                        Input: String (URL), String (Operation Name), <br />
                        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Array of Strings (Parameters)<br/>
                        Output: Method Invocation result<br/>
                    </td>
                    <td>
                        Given a WSDL URL, this service will extract method and <br/>
                        and parameter information and invoke the requested method <br />
                        and will return the result.
                    </td>
                    <td>
                        All libraries required are present within .Net and<br/>
                        no external services are required.
                    </td>
                    <td>
                        <b>NA</b>
                    </td>
                </tr>
                <tr></tr>
                <tr>
                    <td>
                        <br /><i>WSDL Service Testing Service - Group Invoke</i><br/>
                        Input: String (URL), Array of Strings (Input Parameters), <br />
                        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Array of Strings (Outputs)<br/>
                        Output: Method Invocation result<br/>
                    </td>
                    <td>
                        Given a WSDL URL, this service will extract method and <br/>
                        and parameter information and invoke all methods with <br />
                        the input parameters provided and compare with the outputs<br />
                        provided and return the result.
                    </td>
                    <td>
                        All libraries required are present within .Net and<br/>
                        no external services are required.
                    </td>
                    <td>
                        <b>NA</b>
                    </td>
                </tr>
            </table>
        </div>
    </div>
</asp:Content>
