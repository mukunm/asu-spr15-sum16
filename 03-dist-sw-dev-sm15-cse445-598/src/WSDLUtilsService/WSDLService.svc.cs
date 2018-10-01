using Microsoft.CSharp;
using System;
using System.CodeDom;
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime.Serialization;
using System.Reflection;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;
using System.Text.RegularExpressions;
using System.Web.Services.Description;
using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;
using System.Web.Services;
using System.Runtime.Serialization.Formatters.Binary;
using System.Runtime.Serialization.Formatters.Soap;

namespace WSDLUtilitiesServices
{
    public class WSDLService : IWSDLService
    {
        /* Given a Web Service URL, which contains a WSDL file in XML format, this method       */
        /* extracts the method details like the name, input and output parameters               */
        public MethodDetails[] ExtractWsdlOperation(string url)
        {
            List<MethodDetails> serviceMethods = new List<MethodDetails>();

            try
            {
                /* Create a URI builder to read from the given URL */
                UriBuilder webServiceUri = new UriBuilder(new Uri(url));
                webServiceUri.Query = "WSDL";

                /* Create a HttpWebRequest object to read the service description using the in built */
                /* ServiceDescription class.                                                         */
                HttpWebRequest webServiceRequest = (HttpWebRequest)WebRequest.Create(webServiceUri.Uri);
                webServiceRequest.ContentType = "text/xml;charset=\"utf-8\"";
                webServiceRequest.Method = "GET";
                webServiceRequest.Accept = "text/xml";

                /* Create a stream from the given URI to enable the service description class to     */
                /* the WSDL inside.                                                                  */
                WebResponse webServiceResponse = webServiceRequest.GetResponse();
                Stream webServiceStream = webServiceResponse.GetResponseStream();

                /* Create the ServiceDescription object and read from the input stream */
                ServiceDescription serviceDescription = ServiceDescription.Read(webServiceStream);

                /* The service description extracted consists of the following details               */
                /* Port Types                                                                        */
                /* Operation                                                                         */
                /* Messages                                                                          */
                /* Types                                                                             */
                /* The operation name information is easily extracted. However, in order to get the  */
                /* input and output parameter types separately, the flat list that the types list    */
                /* has to be used and the XML out of it has to be parsed.                            */
                /* The loop below traverses through each operation in every port type and extracts   */
                /* all operation names, input, output parameters and their types and adds them to a  */
                /* list which is returned.                                                           */
                foreach (PortType portType in serviceDescription.PortTypes)
                {
                    foreach (Operation operation in portType.Operations)
                    {
                        /* Extract the operation name, input and output message names */
                        string operationName = operation.Name;
                        string inputMessage = operation.Messages.Input.Message.Name;
                        string outputMessage = operation.Messages.Output.Message.Name;

                        /* Extract the input and output part names */
                        string inputPartName = serviceDescription.Messages[inputMessage].Parts[0].Element.Name;
                        string outputPartName = serviceDescription.Messages[outputMessage].Parts[0].Element.Name;

                        /* Extract the input and output parameter names */
                        ParameterDetails[] inputParameters =
                            GetWebMethodParameterDetails(serviceDescription, inputPartName);
                        ParameterDetails[] outputParameters =
                            GetWebMethodParameterDetails(serviceDescription, outputPartName);

                        serviceMethods.Add(new MethodDetails(operationName, inputParameters, outputParameters));
                    }
                }
            }
            catch (Exception ec)
            {
                throw new FaultException(ec.Message);
            }

            return serviceMethods.ToArray();
        }

        /* Given a service description and a message part name, this method extracts */
        /* all the parameter names and types for the particular message              */
        private ParameterDetails[]
            GetWebMethodParameterDetails(ServiceDescription serviceDescription,
                                         string messagePartName)
        {
            List<ParameterDetails> parametersList = new List<ParameterDetails>();

            /* As described in the comment above, the service description library    */
            /* gives out a flat types list and hence, it is required to parse the    */
            /* XML to extract types for each input and output message which is done  */
            /* below.                                                                */

            /* Extract types from the serviceDescription */
            Types types = serviceDescription.Types;

            /* Extrac the XML Schema for the types */
            XmlSchema typesXMLSchema = types.Schemas[0];

            /* Traverse all items in the XML schema. If the message name matches    */
            /* the element name, extract the type for the particular message.       */
            foreach (object item in typesXMLSchema.Items)
            {
                XmlSchemaElement xmlSchemaElement = item as XmlSchemaElement;

                if (xmlSchemaElement == null)
                    continue;

                /* If a match is found, proceed */
                if (xmlSchemaElement.Name == messagePartName)
                {
                    XmlSchemaType xmlSchemaType = xmlSchemaElement.SchemaType;
                    XmlSchemaComplexType xmlSchemaComplexType = xmlSchemaType as XmlSchemaComplexType;

                    /* If its not a complex type, don't proceed */
                    if (xmlSchemaComplexType == null)
                        continue;

                    XmlSchemaParticle xmlParticle = xmlSchemaComplexType.Particle;
                    XmlSchemaSequence xmlSequence = xmlParticle as XmlSchemaSequence;

                    /* If it's not a sequence, don't proceed */
                    if (xmlSequence == null)
                        continue;

                    /* Traverse through all the elements in the sequence and extract */
                    /* parameter types and names. As and when found, add them to the */
                    /* list.                                                         */
                    foreach (XmlSchemaElement element in xmlSequence.Items)
                    {
                        string paramName = element.Name;
                        string paramType = element.SchemaTypeName.Name;
                        parametersList.Add(new ParameterDetails(paramType, paramName));
                    }
                }
            }

            return parametersList.ToArray();
        }
        
        /* Given a URL, this method extracts the content of the page in the URL and obtains all */
        /* WSDL URLs from that page.                                                            */
        public string[] ExtractWsdlAddresses(string url)
        {
            try
            {
                List<string> wsdlAddresses = new List<string>();

                /* Extract the webcontent from the provided URL */
                string webContent = (new WebClient()).DownloadString(url);

                /* Pattern to match all WSDL URLs */
                string pattern = @"(http|https):\/\/[^\s]*?.\?(wsdl|WSDL)";

                /* Match the web content with the regular expression */
                Regex r = new Regex(pattern);
                MatchCollection matches = r.Matches(webContent);

                /* Traverse through the obtained matches and add non-duplicates to the list */
                if (matches.Count > 0)
                {
                    foreach (Match match in matches)
                    {
                        string matchingValue = match.Value;

                        if (wsdlAddresses.Contains(matchingValue) == false)
                            wsdlAddresses.Add(matchingValue);
                    }
                }

                return wsdlAddresses.ToArray();
            }
            catch(Exception ec)
            {
                throw new FaultException(ec.Message);
            }
        }

        /* Given a URL, this method extracts the service type and the methods in the service    */
        /* The method and service type objects are returned to enable the caller to call the    */
        /* required service.                                                                    */
        private void GetWebServiceMethods(string url, out MethodInfo[] methodsInfo, 
                                          out Type serviceType)
        {
            try
            {
                methodsInfo = null;
                serviceType = null;

                /* Create a new URI Object with the URL provided */
                Uri uri = new Uri(url);

                /* Create a web request to the URI to obtain the WSDL file from the URI */
                WebRequest webRequest = WebRequest.Create(uri);
                Stream wsdlRequestStream = webRequest.GetResponse().GetResponseStream();
                ServiceDescription serviceDescription = ServiceDescription.Read(wsdlRequestStream);
                Service service = serviceDescription.Services[0];

                string serviceDescriptionName = service.Name;

                /* Import service description to create a proxy for the methods present in */
                /* the service. These proxies will later on be used to invoke each of those*/
                /* services.                                                               */
                ServiceDescriptionImporter servImport = new ServiceDescriptionImporter();
                servImport.AddServiceDescription(serviceDescription, String.Empty, String.Empty);
                servImport.ProtocolName = "Soap";
                servImport.CodeGenerationOptions = CodeGenerationOptions.GenerateProperties;

                /* Create the compile unit and add the required namespaces to it. */
                CodeNamespace nameSpace = new CodeNamespace();
                CodeCompileUnit codeCompileUnit = new CodeCompileUnit();
                codeCompileUnit.Namespaces.Add(nameSpace);

                /* Obtain the warnings from the compile unit */
                ServiceDescriptionImportWarnings warnings = servImport.Import(nameSpace, codeCompileUnit);

                /* If the warnings obtained are not empty, return */
                if (warnings != 0)
                    return;

                /* Create the string writer object to obtain the the Code DOM for the specified namespace */
                StringWriter stringWriter = new StringWriter(System.Globalization.CultureInfo.CurrentCulture);
                CSharpCodeProvider prov = new CSharpCodeProvider();
                prov.GenerateCodeFromNamespace(nameSpace, stringWriter, new CodeGeneratorOptions());

                /* Set all the required parameters and perform a compilation */
                string[] assemblyReferences = new string[2] { "System.Web.Services.dll", "System.Xml.dll" };
                CompilerParameters param = new CompilerParameters(assemblyReferences);
                param.GenerateExecutable = false;
                param.GenerateInMemory = true;
                param.TreatWarningsAsErrors = false;
                param.WarningLevel = 4;

                CompilerResults results = prov.CompileAssemblyFromDom(param, codeCompileUnit);
                Assembly assembly = results.CompiledAssembly;

                serviceType = assembly.GetType(serviceDescriptionName);
                methodsInfo = serviceType.GetMethods();
            }
            catch (Exception ec)
            {
                throw new Exception(ec.Message);
            }
            
        }

        /* Given a URL, this method extracts the service type and the methods in the service    */
        /* and encodes them into an array of strings.                                           */
        public string[] GetWebServiceMethodsInfo(string url)
        {
            try
            {
                List<string> methodInfo = new List<string>();

                /* Extract the service and the method info from the service */
                Type serviceType;
                MethodInfo[] methodsInfo;

                /* Extract web service methods */
                GetWebServiceMethods(url, out methodsInfo, out serviceType);

                /* Add the method Info name and parameter names and types to the list */
                methodInfo.Add(serviceType.Name);
                foreach (MethodInfo method in methodsInfo)
                {
                    methodInfo.Add("MethodName:" + method.Name);
                    methodInfo.Add("ReturnType:" + method.ReturnType);

                    ParameterInfo[] parameters = method.GetParameters();
                    foreach (ParameterInfo parameter in parameters)
                    {
                        methodInfo.Add("PName:" + parameter.Name);
                        methodInfo.Add("PType:" + parameter.ParameterType.FullName);
                    }
                }

                return methodInfo.ToArray();
            }
            catch(Exception ec)
            {
                throw new FaultException(ec.Message);
            }
        }

        /* Given a service URL, operation name and input parameters for the same, this method   */
        /* extracts the methods of the service and invokes the method requested by passing the  */
        /* parameters requested. The return value is returned to the caller.                    */
        public ReturnData InvokeWebServiceMethod(string url, string operationName, 
                                                 string[] parameters)
        {
            try
            {
                /* Extract the service and the method info from the service */
                Type serviceType;
                MethodInfo[] methodsInfo;

                /* Extract web service methods */
                GetWebServiceMethods(url, out methodsInfo, out serviceType);

                /* If either of methodsInfo or serviceType are null, return */
                if (methodsInfo == null || serviceType == null)
                    return null;

                /* Traverse through the list of methods info and find a method name */
                /* that matches the requested operation name */
                MethodInfo methodToInvoke = methodsInfo[0];
                bool matchFound = false;

                foreach (MethodInfo method in methodsInfo)
                {
                    if (method.Name == operationName)
                    {
                        methodToInvoke = method;
                        matchFound = true;
                        break;
                    }
                }

                /* If no match was found, return */
                if (matchFound == false)
                    return null;

                ReturnData responseData = 
                    InvokeWebServiceMethod(serviceType, methodToInvoke, parameters);
                return responseData;
            }
            catch(Exception ec)
            {
                throw new FaultException(ec.Message);
            }
        }

        /* Given a service URL, operation name and input parameters for the same, this method   */
        /* extracts the methods of the service and invokes the method requested by passing the  */
        /* parameters requested. The return value is returned to the caller.                    */
        private ReturnData InvokeWebServiceMethod(Type serviceType, MethodInfo methodToInvoke, 
                                                  string[] parameters)
        {

            try
            {
                ReturnData responseData = new ReturnData();

                /* Traverse the input parameters provided and convert each of them to the type */
                /* as required by the method to be invoked. */
                Type returnType = methodToInvoke.ReturnType;
                ParameterInfo[] inputParameterTypes = methodToInvoke.GetParameters();
                Object[] inputParameters = new Object[inputParameterTypes.Length];

                for (int i = 0; i < inputParameterTypes.Length; i++)
                {
                    inputParameters[i] = Convert.ChangeType(parameters[i],
                                                            inputParameterTypes[i].ParameterType);
                }

                /* Create a service instance and invoke the requested method */
                Object serviceInstance = Activator.CreateInstance(serviceType);
                Object response = methodToInvoke.Invoke(serviceInstance, inputParameters);

                /* Create the response data object, set the type name and serialize the object to */
                /* XML format.                                                                    */
                responseData.TypeName = returnType.Name;
                XmlSerializer responseSerializer = new XmlSerializer(response.GetType());
                StringWriter responseOutputWriter = new StringWriter();
                responseSerializer.Serialize(responseOutputWriter, response);
                responseData.objectXML = responseOutputWriter.ToString();

                return responseData;
            }
            catch (Exception ec)
            {
                throw new FaultException(ec.Message);
            }
        }

        /* Given 2 XML nodes, this method, traverses the node structure recursively checking    */
        /* for element, attribute or content mismatches at any node in the tree. If any of them */
        /* don't match, it returns a failure result. If all of them match, it returns a success */
        /* as the result . */
        private bool compareXmlDocuments(XmlNode xmlNode1, XmlNode xmlNode2)
        {
            bool validationStatus = false;
            try
            {
                /* If the Node names don't match return a failure status */
                if (xmlNode1.Name != xmlNode2.Name)
                    return validationStatus;

                /* Compare attributes of the node if both attributes are non null */
                if (xmlNode1.Attributes != null && xmlNode2.Attributes != null)
                {
                    /* If the attribute counts don't match, return a failure status */
                    if (xmlNode1.Attributes.Count != xmlNode2.Attributes.Count)
                        return validationStatus;

                    XmlAttributeCollection xmlAttributes1 = xmlNode1.Attributes;
                    XmlAttributeCollection xmlAttributes2 = xmlNode2.Attributes;

                    /* Traverse through the attribute lists, compare their names and values */
                    /* If either of them don't match, return a failure status */
                    for (int i = 0; i < xmlAttributes1.Count; i++)
                    {
                        XmlAttribute xmlAttribute1 = (XmlAttribute)xmlAttributes1.Item(i);
                        XmlAttribute xmlAttribute2 = (XmlAttribute)xmlAttributes2.Item(i);

                        if (xmlAttribute1.Name != xmlAttribute2.Name)
                            return validationStatus;

                        if (xmlAttribute2.Value != xmlAttribute2.Value)
                            return validationStatus;
                    }
                }
                else
                {
                    /* If one of the nodes has no attributes and the other has attributes */
                    /* return a failure status. */
                    if (xmlNode1.Attributes == null && xmlNode2.Attributes != null)
                    {
                        return validationStatus;
                    }
                    else if (xmlNode1.Attributes != null && xmlNode2.Attributes == null)
                    {
                        return validationStatus;
                    }
                    else
                    {
                        // Do Nothing because attributes of both xml nodes are non null */
                    }
                }

                /* If the contents in the nodes don't match return an error */
                if (xmlNode1.Value != xmlNode2.Value)
                    return validationStatus;

                /* If one of them doesn't have child nodes and the other one doesn't, */
                /* return a failure status */
                if (xmlNode1.HasChildNodes != xmlNode2.HasChildNodes)
                    return validationStatus;

                XmlNodeList children1 = xmlNode1.ChildNodes;
                XmlNodeList children2 = xmlNode2.ChildNodes;

                /* If the number of children for the 2 nodes don't match, return error */
                if (children1.Count != children2.Count)
                    return validationStatus;

                /* Traverse through all the child nodes and compare the child nodes by calling this */
                /* method recursively for each child node found. */
                for (int i = 0; i < children1.Count; i++)
                {
                    validationStatus = compareXmlDocuments(children1.Item(i), children2.Item(i));

                    /* If any of the comparisons, returns a failure, return a failure and don't */
                    /* proceed further */
                    if (validationStatus == false)
                        return validationStatus;
                }

                /* If none of the failures occur, mark the status as true and return */
                validationStatus = true;
                return validationStatus;
            }
            catch(Exception ec)
            {
                return validationStatus;
            }
        }

        /* Given, 2 XML strings, this method performs an exact element-to-element,              */
        /* attribute-to-attribute and content-to-content comparison between the 2 XML strings   */
        /* and returns the comparison status */
        private bool compareXmlStrings(string xmlString1, string xmlString2)
        {
            bool validationStatus = false;

            try
            {
                /* Create new XML documents for each of the strings passed */
                XmlDocument xmlDocument1 = new XmlDocument();
                XmlDocument xmlDocument2 = new XmlDocument();

                /* Load the xml strings into the XML documents */
                xmlDocument1.LoadXml(xmlString1);
                xmlDocument2.LoadXml(xmlString2);

                /* Extract the root node from each of the documents */
                XmlNode xmlNode1 = xmlDocument1.DocumentElement;
                XmlNode xmlNode2 = xmlDocument2.DocumentElement;

                /* Compare the documents to obtain the validation status */
                validationStatus = compareXmlDocuments(xmlNode1, xmlNode2);

                return validationStatus;
            }
            catch(Exception ec)
            {
                return validationStatus;
            }
        }

        /* Given a service URL, a set of input parameters and a set of expected Output          */
        /* this method calls each operation from the service with the provided input parameters */
        /* matches the obtained outputs with the outputs provided and declares the result of the*/
        /* operation which is then returned.                                                    */
        /* Note that the output parameters need to be provided as an XML string because the     */
        /* deserialization cannot take care of deserializing complex objects to perform the     */
        /* comparison.                                                                          */
        public InvokeResult InvokeWebServiceOperationsGroup(string url, string[] inputParams, 
                                                            string[] expectedOutputParams)
        {
            try
            {
                /* Extract the service and the method info from the service */
                Type serviceType;
                MethodInfo[] methodsInfo;

                /* Extract web service methods */
                GetWebServiceMethods(url, out methodsInfo, out serviceType);

                /* If either of the methods info or the service type weren't obtained, return */
                if (methodsInfo == null || serviceType == null)
                    return null;

                List<MethodInfo> methodsToInvoke = new List<MethodInfo>();
                int inputParametersRequired = 0;

                /* Create a list of methods to be invoked */
                foreach (MethodInfo method in methodsInfo)
                {
                    if (method.Name == "Discover")
                        break;

                    methodsToInvoke.Add(method);
                    inputParametersRequired += method.GetParameters().Length;
                }

                /* If the number of output parameters passed don't match with the number of */
                /* methods to be invoked, return */
                if (methodsToInvoke.Count != expectedOutputParams.Length)
                    return null;

                /* The provided input parameters are not sufficient to make all invocations */
                if (inputParametersRequired != inputParams.Length)
                    return null;

                InvokeResult invokeResults = new InvokeResult(expectedOutputParams.Length);

                int j = 0;
                int k = 0;
                foreach (MethodInfo methodToInvoke in methodsToInvoke)
                {
                    ParameterInfo[] invokeMethodParameters = methodToInvoke.GetParameters();
                    string[] invokeMethodInputParameters = new string[invokeMethodParameters.Length];

                    /* The total number of input parameters provided should be equal to the summation */
                    /* of the total number of input parameters provided for each method. If not, there*/
                    /* has been some error in parsing the input parameters and an error has to be     */
                    /* returned. If this case is hit, it is a bug and will need to be debugged.       */
                    if (j >= inputParams.Length)
                        return null;

                    for (int i = 0; i < invokeMethodInputParameters.Length; i++)
                    {
                        invokeMethodInputParameters[i] = inputParams[j];
                        j++;
                    }

                    /* Invoke web service method */
                    ReturnData responseData = InvokeWebServiceMethod(serviceType, methodToInvoke,
                                                                     invokeMethodInputParameters);

                    /* Check if the returned data matches with that provided */
                    if (compareXmlStrings(responseData.objectXML, expectedOutputParams[k]) == false)
                        invokeResults.InvokeStatus[k] = false;
                    else
                        invokeResults.InvokeStatus[k] = true;

                    invokeResults.MethodName[k] = methodToInvoke.Name;

                    k++;
                }

                return invokeResults;
            }
            catch(Exception ec)
            {
                throw new FaultException(ec.Message);
            }
        }

    }
}

