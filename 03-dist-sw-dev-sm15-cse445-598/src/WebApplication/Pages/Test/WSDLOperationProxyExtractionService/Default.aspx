<%@ Page Title="WSDL Hash Operations" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeFile="Default.aspx.cs" Inherits="_Default" %>

<asp:Content ID="BodyContent" ContentPlaceHolderID="MainContent" runat="server">

    <div class="jumbotron">
        <h1>WSDL Hash Operations</h1>
        <p class="lead">
            This service enables a user to extract all the operations supported by a SOAP service by extractin them out of a WSDL file. Enter the WSDL URL and hit proceed to extract all the operations.
            <br />
            <br />
            Note: This page is very similar to the WSDL Operations page. But, the service returns the operation objects through which the operations of the service can be invoked. This page will be extended to include Service invocation in later stages.</p>
        <p class="lead">
            <asp:TextBox ID="webPageUrlTextBox" runat="server" Height="37px" Width="347px"></asp:TextBox>
&nbsp;
            <asp:Button ID="proceedButton" runat="server" OnClick="proceedButton_Click" Text="Proceed" />
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            <asp:Label ID="errorMessageLabel" runat="server"></asp:Label>
        </p>
    </div>

    <asp:PlaceHolder ID="operationsPlaceHolder" runat="server"></asp:PlaceHolder>

    </asp:Content>
