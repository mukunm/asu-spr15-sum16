<%@ Page Title="SOAP Explore - Search" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeFile="Search.aspx.cs" Inherits="Search" %>

<asp:Content ID="BodyContent" ContentPlaceHolderID="MainContent" runat="server">
    <div class="jumbotron">
        <h1>SOAP Search Engine</h1>
        <p class="lead">Welcome to the SOAP Search engine. 
            <br />You can use this page to discover SOAP services by providing a keyword that matches the required service.
        </p>
    </div>

    <p>
        <asp:Label ID="searchKeywordLabel" runat="server" Text="Enter a keyword" style="font-weight: 700">
        </asp:Label>
    </p>
    <p>
        <asp:TextBox ID="searchKeywordTextBox" runat="server" Width="750px" Height ="30px"> 
        </asp:TextBox>
        <asp:Button ID="searchButton" runat="server" 
                    Height="30px" Text="Search" 
                    Width="75px" OnClick="searchButton_Click"
                    CssClass="btn btn-default"/>                
    </p>

    <p class="text-danger">
        <asp:Label ID="errorMessageLabel" runat="server"></asp:Label>
    </p>

    <asp:DataList ID="searchResultsList" runat="server">
        <ItemTemplate>
            <p>
                <b>
                    <asp:hyperlink Id="titleHyperLink" 
                                   runat="server" 
                                   NavigateUrl='<%# DataBinder.Eval(Container.DataItem, "link") %>'
                                   Target="_blank"
                                   Text='<%# DataBinder.Eval(Container.DataItem, "title") %>'>
                    </asp:hyperlink>
                </b>
                &nbsp;&nbsp;&nbsp;
                <asp:hyperlink Id="tryItLink" 
                                runat="server" 
                                NavigateUrl='<%# "~/ServiceInvoke.aspx?serviceWsdlLink=" + DataBinder.Eval(Container.DataItem, "link") %>'
                                Target="_blank"
                                Text="Try a sample Invocation">
                </asp:hyperlink>
                <br />
                <%# DataBinder.Eval(Container.DataItem, "link") %>
                <br />
                <b>Operations in brief: </b> <%# DataBinder.Eval(Container.DataItem, "methodDetails") %>
                <hr />
            </p>
        </ItemTemplate>
    </asp:DataList>
</asp:Content>
