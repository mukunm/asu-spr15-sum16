<%@ Page Title="SOAP Explore" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeFile="Default.aspx.cs" Inherits="_Default" %>

<asp:Content ID="BodyContent" ContentPlaceHolderID="MainContent" runat="server">
    <div class="jumbotron">
    <h1>Welcome to SOAP Explore!</h1>
    <p>
        Looking for SOAP services? Discover, Invoke and Test through SOAP Explore
    </p>
    <hr />
    <h5>
        Have something to say about SOAP explore? Feel free to 
        <asp:hyperlink Text="write us an e-mail." NavigateUrl="~/Contact.aspx" runat="server">
        </asp:hyperlink>
    </h5>
    </div>
    <div class="row">
        <h2>Architecture Overview</h2>
        <div class="col-md-12">
            <img src="Architecture.jpg" alt="Architecture Overview" width="1100"/>
        </div>
    </div>
    
    <div class="row">
        <h2>Need more information?</h2>
        <div class="col-md-4">
            <h4>More about SOAP Explore</h4>
            <ul>
                <li>
                    Let's say you're a developer and you are looking for services to integrate 
                    in your application
                </li>
                <li>
                    You use the search engine at SOAP Explore, enter a keyword that closely
                    relates to the service you're looking to find.
                </li>
                <li>
                    Go through the entire list of Services that SOAP explore produces,
                    test invoke them and use the service that best suits your requirements.
                </li>
            </ul>
        </div>
        <div class="col-md-4">
            <h4>If you're looking to use the system</h4>
            <ul>
                <li> Start by  
                    <asp:HyperLink runat="server" NavigateUrl="~/Account/Register.aspx" Text="Registering">
                    </asp:HyperLink>
                    if you're new to the service or by 
                    <asp:HyperLink runat="server" NavigateUrl="~/Account/Login.aspx" Text="Logging in">
                    </asp:HyperLink>
                    if you already have a user account
                </li>
                <li>
                    Start using the system by proceeding to the
                    <asp:HyperLink runat="server" NavigateUrl="~/Search.aspx" Text="Search Page">
                    </asp:HyperLink>
                </li>
                <li>
                    You could proceed to the 
                    <asp:HyperLink runat="server" NavigateUrl="~/ServiceInvoke.aspx" Text="Service Invoke Page">
                    </asp:HyperLink>
                    if the search doesn't interest you.
                </li>
            </ul>
        </div>
        <div class="col-md-4">
            <h4>If you're looking to use the services</h4>
            <p>
                For a detailed description, take a look at
                <asp:HyperLink runat="server" NavigateUrl="~/Test.aspx" Text="Test Pages">
                </asp:HyperLink>
            </p>
            <p>
                The following services are available
            </p>
            <ol>
                <li>WSDL Discovery Service</li>
                <li>WSDL Address Extraction Service</li>
                <li>WSDL Operation Extraction Service</li>
                <li>WSDL Operation Proxy Extraction Service</li>
                <li>WSDL Service Testing Service - Single Invoke</li>
                <li>WSDL Service Testing Service - Group Invoke</li>
            </ol>
        </div>
    </div>

</asp:Content>
