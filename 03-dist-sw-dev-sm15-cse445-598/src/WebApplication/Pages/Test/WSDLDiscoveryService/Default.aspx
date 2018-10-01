<%@ Page Title="WSDL Search Service" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeFile="Default.aspx.cs" Inherits="_Default" %>

<asp:Content ID="BodyContent" ContentPlaceHolderID="MainContent" runat="server">

    <div class="jumbotron">
        <h1>WSDL Discovery Service Description</h1>
        <p class="lead">
            This service enables a user to discover services available for any required purpose. 
            Use the text box below to enter keywords related to the service that you are looking 
            for and hit the search button to proceed.&nbsp;&nbsp;&nbsp;
        </p>
        <p class="lead">
            <asp:TextBox ID="searchTextBox" runat="server" Height="37px" Width="347px"></asp:TextBox>
&nbsp;
            <asp:Button ID="searchButton" runat="server" OnClick="searchButton_Click" Text="Search" />
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            <asp:Label ID="errorMessageLabel" runat="server"></asp:Label>
        </p>
    </div>

    <asp:PlaceHolder ID="searchResultsPlaceHolder" runat="server"></asp:PlaceHolder>

    </asp:Content>
