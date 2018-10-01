<%@ Page Title="SOAP Explore - Service Invoke" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeFile="ServiceInvoke.aspx.cs" Inherits="ServiceInvoke" ValidateRequest="false"%>

<asp:Content ID="BodyContent" ContentPlaceHolderID="MainContent" runat="server">

    <div class="jumbotron">
        <h1>Service Invoke</h1>
        <p class="lead">Welcome to Service Invoke.
             <br />You can use this page to invoke service operations provided you have the URL to the SOAP service description page (WSDL)
        </p>
    </div>
    
    <p>
        <asp:Label ID="serviceUrlLabel" runat="server" Text="Enter Service URL Here" style="font-weight: 700"></asp:Label>
    </p>

    <p>
        <asp:TextBox ID="serviceUrlTextBox" runat="server" Width="750px"></asp:TextBox>
        <asp:Button ID="exploreOperationsButton" runat="server" Height="30px" Text="Explore operations" Width="162px" OnClick="exploreOperationsButton_Click"  CssClass="btn btn-default"/>
    </p>

    <p class="text-danger">
        <asp:Label ID="errorMessageLabel" runat="server"></asp:Label>
    </p>

    <div class="row">
        <div class="col-md-4">
            <asp:Panel ID="serviceOperationsPanel" runat="server">
                <asp:Label ID="operationsLabel" runat="server" Font-Bold="True"></asp:Label>
                &nbsp;<asp:TreeView ID="operationsTree" runat="server" OnSelectedNodeChanged="operationsTree_SelectedNodeChanged">
                </asp:TreeView>
                <asp:RadioButtonList ID="invokeTypeButtonList" runat="server" Height="20px" RepeatDirection="Horizontal" Width="267px">
                    <asp:ListItem Value="SingleInvoke">Single Invoke</asp:ListItem>
                    <asp:ListItem Value="GroupInvoke">Group Invoke</asp:ListItem>
                </asp:RadioButtonList>
                <asp:Label ID="instructionMessageLabel" runat="server">To proceed with invocation select one of the 2 invocation options and click on the operation that you wish to execute.</asp:Label>
                <br />
                <br />
                &nbsp;<asp:Button ID="invokeButton" runat="server" Height="27px" OnClick="invokeButton_Click" Text="Invoke Operation" Width="155px" CssClass="btn btn-default"/>
                <br />
            </asp:Panel>
        </div>

        <div class="col-md-4">
            <asp:Label ID="parametersLabel" runat="server" Font-Bold="True"></asp:Label>
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            <asp:DataList ID="parameterValueDataList" runat="server">
                <ItemTemplate>
                    <table style="border:none">
                        <tr>
                            <td><b>Parameter Name</b></td>
                            <td><b>Parameter Type</b></td>
                            <td><b>Parameter Value</b></td>
                        </tr>
                        <tr>
                            <td><b><%# Eval("parameterName") %></b></td>
                            <td></b><%# Eval("parameterType") %></b></td>
                            <td><asp:TextBox ID="parameterValue" runat="server"></asp:TextBox></td>
                        </tr>
                    </table><br />
                </ItemTemplate>
            </asp:DataList>
            <br />

            <asp:Label ID="resultsLabel" runat="server" Font-Bold="True"></asp:Label>
            <asp:Panel ID="resultsPanel" runat="server">
            </asp:Panel>
        </div>
        
        <div class="col-md-4">
            <asp:Label ID="groupInvokeParametersLabel" runat="server" Font-Bold="True"></asp:Label>
            <br />
            <br />

            <asp:Label ID="groupInvokeParametersInstructionLabel" runat="server" Font-Bold="False"></asp:Label>
            <br />
            <br />

            <asp:Label ID="groupInvokeParametersInputLabel" runat="server" Font-Bold="False"></asp:Label>
            <br />

            <asp:Label ID="groupInvokeParametersOutputLabel" runat="server" Font-Bold="False"></asp:Label>
            <br />
            <br />

            <asp:Label ID="groupInvokeParametersHintLabel" runat="server" Font-Bold="True"></asp:Label>
            <br />
            <br />

            <asp:DataList ID="groupInvokeList" runat="server" OnItemDataBound="groupInvokeList_ItemDataBound">
                <ItemTemplate>
                    <table style="border:none">
                        <tr>
                            <td>
                                <b>Method Name - <%# DataBinder.Eval(Container.DataItem, "methodName") %></b>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <asp:DataList ID="groupInvokeInputParameterList" runat="server" DataSource='<%# DataBinder.Eval(Container.DataItem, "inputParameters") %>'>
                                    <ItemTemplate>
                                        <table style="border:none">
                                            <tr>
                                                <td>
                                                    <b>Input Parameter - <%# DataBinder.Eval(Container.DataItem, "parameterName") %>; Type - <%# DataBinder.Eval(Container.DataItem, "parameterType") %></b>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td>
                                                    <asp:TextBox ID="groupInvokeInputParameterValue" TextMode="multiline" Columns="50" Rows="5" runat="server"></asp:TextBox>
                                                </td>
                                            </tr>
                                        </table>
                                    </ItemTemplate>
                                </asp:DataList>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <asp:DataList ID="groupInvokeOutputParameterList" runat="server" DataSource='<%# DataBinder.Eval(Container.DataItem, "outputParameters") %>'>
                                    <ItemTemplate>
                                        <table style="border:none">
                                            <tr>
                                                <td>
                                                    <b>Output Parameter - <%# DataBinder.Eval(Container.DataItem, "parameterName") %></b>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td>
                                                    <asp:TextBox ID="groupInvokeOutputParameterValue" TextMode="multiline" Columns="50" Rows="5" runat="server"></asp:TextBox>
                                                </td>
                                            </tr>
                                        </table>
                                    </ItemTemplate>
                                </asp:DataList>
                            </td>
                        </tr>
                    </table>
                </ItemTemplate>
            </asp:DataList>

            <br />

            <asp:Label ID="groupInvokeResultsLabel" runat="server" Font-Bold="True"></asp:Label>
            <asp:Panel ID="groupInvokeResultsPanel" runat="server">
            </asp:Panel>
        </div>
    </div>

    <br />
</asp:Content>



