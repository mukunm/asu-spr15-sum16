using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;
using System.Reflection;
using System.IO;
using System.Xml.Serialization;

namespace WSDLUtilitiesServices
{
    /* This interface contains all WSDL utility services required for */
    /* a WSDL exploratory system.                                     */
    [ServiceContract]
    public interface IWSDLService
    {
        [OperationContract]
        MethodDetails[] ExtractWsdlOperation(string url);

        [OperationContract]
        string[] ExtractWsdlAddresses(string url);

        [OperationContract]
        string[] GetWebServiceMethodsInfo(string url);

        [OperationContract]
        ReturnData InvokeWebServiceMethod(string url, string operationName, 
                                          string[] parameters);

        [OperationContract]
        InvokeResult InvokeWebServiceOperationsGroup(string url, string[] inputParams, 
                                                     string[] expectedOutputParams);
    }

    public struct MethodDetails
    {
        public string methodName;
        public ParameterDetails[] inputParameters;
        public ParameterDetails[] outputParameters;

        /* Default constructor that initializes the method details */
        public MethodDetails(string methodName,
                             ParameterDetails[] inputParameters,
                             ParameterDetails[] outputParameters)
        {
            this.methodName = methodName;
            this.inputParameters = inputParameters;
            this.outputParameters = outputParameters;
        }
    }

    public struct ParameterDetails
    {
        public string parameterType;
        public string parameterName;

        /* Default constructor that initializes the parameter details */
        public ParameterDetails(string parameterType,
                                string parameterName)
        {
            this.parameterType = parameterType;
            this.parameterName = parameterName;
        }
    }

    [Serializable]
    [DataContract]
    public class ReturnData
    {
        [DataMember]
        public string TypeName
        {
            get;
            set;
        }

        [DataMember]
        public string objectXML
        {
            get;
            set;
        }
    }

    [Serializable]
    [DataContract]
    public class InvokeResult
    {
        [DataMember]
        public string[] MethodName
        {
            get;
            set;
        }

        [DataMember]
        public bool[] InvokeStatus
        {
            get;
            set;
        }

        public InvokeResult(int numMethods)
        {
            MethodName = new string[numMethods];
            InvokeStatus = new bool[numMethods];
        }
    }
}
