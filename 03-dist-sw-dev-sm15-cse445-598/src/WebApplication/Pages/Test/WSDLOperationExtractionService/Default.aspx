<%@ Page Title="WSDL Parser Service" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeFile="Default.aspx.cs" Inherits="_Default" %>

<asp:Content ID="BodyContent" ContentPlaceHolderID="MainContent" runat="server">

    <div class="jumbotron">
        <h1>WSDL Parsing Service Description</h1>
        <p class="lead">
            This service enables a user to enter a service URL in WSDL format and list the operations that the service provides.&nbsp; 
            Use the text box below to enter service URL and hit proceed
        </p>
        <p class="lead">
            <asp:TextBox ID="urlTextBox" runat="server" Height="37px" Width="347px"></asp:TextBox>
&nbsp;
            <asp:Button ID="proceedButton" runat="server" OnClick="proceedButton_Click" Text="Proceed" />
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            <asp:Label ID="errorMessageLabel" runat="server"></asp:Label>
        </p>
    </div>

    <asp:PlaceHolder ID="parseResultPlaceHolder" runat="server"></asp:PlaceHolder>

    </asp:Content>
