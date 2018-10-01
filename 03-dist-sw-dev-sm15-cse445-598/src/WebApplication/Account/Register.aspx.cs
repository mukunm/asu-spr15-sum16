using Microsoft.AspNet.Identity;
using System;
using System.Data;
using System.Data.SqlClient;
using System.Web.Security;
using System.IO;
using System.Linq;
using System.Web.UI;
using WebApplication;
using HashingLibrary;

public partial class Account_Register : Page
{
    protected void CreateUser_Click(object sender, EventArgs e)
    {
        if(0 != String.Compare(captchaBox.Text, "v4xbg", true))
        {
            ErrorMessage.Text = "The text doesn't match that in the image.";
            return;
        }

        DataSet ds = new DataSet();
        String userFile = "../App_Data/Users.xml";
        FileStream fs = new FileStream(Server.MapPath(userFile),
            FileMode.Open, FileAccess.Read);
        StreamReader reader = new StreamReader(fs);
        ds.ReadXml(reader);
        fs.Close();
        HashComputer hashComputer = new HashComputer();
        string hashedpwd = hashComputer.hash(Password.Text);
        DataRow newUser = ds.Tables[0].NewRow();
        newUser["UserName"] = UserName.Text;
        newUser["Password"] = hashedpwd;
        newUser["Role"] = roleList.SelectedItem.Text;
        ds.Tables[0].Rows.Add(newUser);
        ds.AcceptChanges();

        fs = new FileStream(Server.MapPath(userFile), FileMode.Create,
                            FileAccess.Write | FileAccess.Read);
        StreamWriter writer = new StreamWriter(fs);
        ds.WriteXml(writer);
        writer.Close();
        fs.Close();

        if (!String.IsNullOrEmpty(Request.QueryString["ReturnUrl"]))
            Response.Redirect(Request.QueryString["ReturnUrl"]);
        else
            Response.Redirect("~/Account/Login");
    }
}