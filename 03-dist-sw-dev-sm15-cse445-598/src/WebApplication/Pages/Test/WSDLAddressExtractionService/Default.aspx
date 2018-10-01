<%@ Page Title="WSDL Address Extraction" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeFile="Default.aspx.cs" Inherits="_Default" %>

<asp:Content ID="BodyContent" ContentPlaceHolderID="MainContent" runat="server">

    <div class="jumbotron">
        <h1>WSDL Address Service Description</h1>
        <p class="lead">
            This service enables a user to extract WSDL URLs from a web page. 
            Use the text box below to enter the URL for a Web Page from which WSDL URLs need to be extracted and hit the proceed button to proceed.&nbsp;&nbsp;&nbsp;
        </p>
        <p class="lead">
            <asp:TextBox ID="webPageUrlTextBox" runat="server" Height="37px" Width="347px"></asp:TextBox>
&nbsp;
            <asp:Button ID="proceedButton" runat="server" OnClick="proceedButton_Click" Text="Proceed" />
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            <asp:Label ID="errorMessageLabel" runat="server"></asp:Label>
        </p>
    </div>

    <asp:PlaceHolder ID="addressResultsPlaceHolder" runat="server"></asp:PlaceHolder>

    </asp:Content>
