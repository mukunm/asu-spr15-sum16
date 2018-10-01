using Microsoft.AspNet.Identity;
using Microsoft.Owin.Security;
using System;
using System.Data;
using System.Data.SqlClient;
using System.Web.Security;
using System.IO;
using System.Web;
using System.Web.UI;
using WebApplication;
using HashingLibrary;

public partial class Account_Login : Page
{
        protected void Page_Load(object sender, EventArgs e)
        {
            RegisterHyperLink.NavigateUrl = "Register";
            var returnUrl = HttpUtility.UrlEncode(Request.QueryString["ReturnUrl"]);
            if (!String.IsNullOrEmpty(returnUrl))
            {
                RegisterHyperLink.NavigateUrl += "?ReturnUrl=" + returnUrl;
            }
        }

        protected void LogIn(object sender, EventArgs e)
        {
            if (IsValid)
            {
                DataSet ds = new DataSet();
                String cmd = "UserName='" + UserName.Text + "'";
                FileStream fs = new FileStream(Server.MapPath("../App_Data/Users.xml"),
                                  FileMode.Open, FileAccess.Read);
                StreamReader reader = new StreamReader(fs);
                ds.ReadXml(reader);
                fs.Close();

                DataTable users = ds.Tables[0];
                DataRow[] matches = users.Select(cmd);

                if( matches != null && matches.Length > 0 )
                {
                    DataRow row = matches[0];
                    HashComputer hashComputer = new HashComputer();
                    string hashedpwd = hashComputer.hash(Password.Text);
                    
                    String pass = (String)row["Password"];
                    if (0 != String.Compare(pass, hashedpwd, false))
                    {
                        // Tell the user if no password match is found. It is good  
                        // security practice give no hints about what parts of the
                        // logon credentials are invalid.
                        FailureText.Text = "Invalid username or password.";
                        ErrorMessage.Visible = true;
                    }
                    else
                    {
                        // If a password match is found, redirect the request
                        // to the originally requested resource (Default.aspx).
                        FormsAuthentication.RedirectFromLoginPage
                            (UserName.Text, RememberMe.Checked);
                    }
                }
                else
                {
                    FailureText.Text = "Invalid username or password.";
                    ErrorMessage.Visible = true;
                }
            }
        }
}