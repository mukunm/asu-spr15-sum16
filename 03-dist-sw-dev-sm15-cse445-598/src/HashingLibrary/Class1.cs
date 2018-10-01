using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Security.Cryptography;

namespace HashingLibrary
{
    public class HashComputer
    {
        public string hash(string password)
        {
            byte[] data = System.Text.Encoding.UTF8.GetBytes(password);
            SHA1 sha = new SHA1CryptoServiceProvider();
            byte[] hashedData = sha.ComputeHash(data);
            string hashValue = System.Text.Encoding.UTF8.GetString(hashedData);
            return hashValue;
        }
    }
}
