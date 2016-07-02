using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;
using System.Diagnostics;
using System.Xml;
using System.IO;

namespace HSQuanTradeMain.StrategyResume
{
    public class ContractPostion
    {
        protected string m_sContractCode;
        public string ContractCode
        {
            get { return m_sContractCode; }
            set { m_sContractCode = value; }
        }
        protected string m_sContractName;
        public string ContractName
        {
            get { return m_sContractName; }
            set { m_sContractName = value; }
        }
        protected int m_sPostion;
        public int Postion
        {
            get { return m_sPostion; }
            set { m_sPostion = value; }
        }
        protected int m_iDirection;
        public int Direction
        {
            get { return m_iDirection; }
            set { m_iDirection = value; }
        }

        protected double m_dbOpenPrice;
        public double OpenPrice
        {
            get { return m_dbOpenPrice; }
            set { m_dbOpenPrice = value; }
        }
        protected string m_sOpenTime;
        public string OpenTime
        {
            get { return m_sOpenTime; }
            set { m_sOpenTime = value; }
        }
        //保存最高价
        protected double m_dbHighPrice;
        public double HighPrice
        {
            get { return m_dbHighPrice; }
            set { m_dbHighPrice = value; }
        }

    }

    public class StrategyParam
    {
        protected string m_sParamName;
        public string ParamName
        {
            get { return m_sParamName; }
            set { m_sParamName = value; }
        }
        protected string m_sParamValue;
        public string ParamValue
        {
            get { return m_sParamValue; }
            set { m_sParamValue = value; }
        }
    }
    public class StrategyContext
    {
        protected string m_sUid;
        public string Uid
        {
            get { return m_sUid; }
            set { m_sUid = value; }
        }
        protected string m_sStrategyName;
        public string StrategyName
        {
            get { return m_sStrategyName; }
            set { m_sStrategyName = value; }
        }

        protected List<StrategyParam> m_arParams = new List<StrategyParam>();
        public List<StrategyParam> Params
        {
            get { return m_arParams; }
            set { m_arParams = value; }
        }


        protected List<ContractPostion> m_arPostions = new List<ContractPostion>();
        public List<ContractPostion> Postions
        {
            get { return m_arPostions; }
            set { m_arPostions = value; }
        }
        
    }

    public class StrategySerial
    {
        

        protected string m_sFilePath = "";
        protected XmlDocument m_oXmlDoc = null;

        public StrategySerial()
        {
            string strPath = Process.GetCurrentProcess().MainModule.FileName;

            int nt = strPath.LastIndexOf("\\");

            strPath = strPath.Substring(0, nt);
            strPath += "\\StrageDLL\\strategy.xml";
            m_sFilePath = strPath;

           
        }


        protected virtual void ParseStrategyContext(XmlNode oXmlNode, ref List<StrategyContext> oLstContext)
        {
            if (0 == oXmlNode.Name.CompareTo("strategy_case"))
            {
                string sStatus = oXmlNode.Attributes["status"].Value;
                if (sStatus != null)
                {
                   
                    if (0 == sStatus.CompareTo("0"))//未加载
                    {
                        StrategyContext oStragtegySer = new StrategyContext();
                        oStragtegySer.Uid = oXmlNode.Attributes["id"].Value;
                        XmlNodeList xmlNodeLst = oXmlNode.ChildNodes;
                        foreach (XmlNode oChildNode in xmlNodeLst)
                        {
                            //if (0 == oChildNode.Name.CompareTo("id"))
                            //{
                            //    oStragtegySer.Uid = oChildNode.FirstChild.Value;
                            //}
                            if (0 == oChildNode.Name.CompareTo("name"))
                            {
                                oStragtegySer.StrategyName = oChildNode.FirstChild.Value;
                            }
                            else if (0 == oChildNode.Name.CompareTo("params"))
                            {
                                XmlNodeList oParamLst = oChildNode.ChildNodes;
                                foreach (XmlNode oParamNode in oParamLst)
                                {
                                    StrategyParam oParam = new StrategyParam();
                                    oParam.ParamName = oParamNode.Attributes["name"].Value;
                                    if (string.Empty == oParam.ParamName)
                                        continue;
                                    oParam.ParamValue = oParamNode.Attributes["value"].Value;
                                    oStragtegySer.Params.Add(oParam);
                                }
                            }
                            else if (0 == oChildNode.Name.CompareTo("contract_postion"))
                            {
                                XmlNodeList oPostionList = oChildNode.ChildNodes;
                                foreach (XmlNode oPostionNode in oPostionList)
                                {
                                    ContractPostion oPostion = new ContractPostion();
                                    oPostion.ContractCode = oPostionNode.Attributes["code"].Value;
                                    oPostion.ContractName = oPostionNode.Attributes["name"].Value;
                                    oPostion.Postion = Int32.Parse(oPostionNode.Attributes["postion"].Value);
                                    oPostion.Direction = Int32.Parse(oPostionNode.Attributes["dir"].Value);
                                    oPostion.OpenPrice = double.Parse(oPostionNode.Attributes["open_price"].Value);
                                    oPostion.OpenTime = oPostionNode.Attributes["open_time"].Value;
                                    if (oPostionNode.Attributes["high_price"] != null)
                                    {
                                        oPostion.HighPrice = double.Parse(oPostionNode.Attributes["high_price"].Value);
                                    }
                                    oStragtegySer.Postions.Add(oPostion);
                                }
                            }

                        }

                        oLstContext.Add(oStragtegySer);
                    }
                }


            }
        }

        protected void MakesureXmlExist()
        {
            if (!File.Exists(m_sFilePath))
            {
                m_oXmlDoc = new XmlDocument();
                if (null != m_oXmlDoc)
                {
                    XmlDeclaration xmlDec = m_oXmlDoc.CreateXmlDeclaration("1.0", "gb2312", null);
                    m_oXmlDoc.AppendChild(xmlDec);

                    XmlElement xmlRoot = m_oXmlDoc.CreateElement("strategy_bak");
                    m_oXmlDoc.AppendChild(xmlRoot);
                    m_oXmlDoc.Save(m_sFilePath);
                }
                
            }
        }

        public bool Load(ref List<StrategyContext> oLstContext)
        {
            MakesureXmlExist();
            try
            {
                m_oXmlDoc = new XmlDocument();
                m_oXmlDoc.Load(m_sFilePath);

                XmlNodeList xmlChildLst = m_oXmlDoc.ChildNodes;
                foreach (XmlNode oChildNode in xmlChildLst)
                {
                    if (0 == oChildNode.Name.CompareTo("strategy_bak"))
                    {
                        XmlNodeList xmlNodeLst = oChildNode.ChildNodes;
                        foreach (XmlNode oXmlNode in xmlNodeLst)
                        {
                            ParseStrategyContext(oXmlNode, ref oLstContext);
                        }
                    }
                }
            }
            catch (System.Exception ex)
            {
                Log.WriteLog("Load" + ex.ToString(), 1);
                return false;
            }
            


            
            
            return true;
        }


        public virtual void Save(StrategyContext oContext)
        {
            MakesureXmlExist();
            try
            {
                m_oXmlDoc = new XmlDocument();
                m_oXmlDoc.Load(m_sFilePath);
            }
            catch (System.Exception ex)
            {
            	//xml format is not correct, remake it
                Log.WriteLog("Save" + ex.ToString(), 1);
                File.Delete(m_sFilePath);
                MakesureXmlExist();

                m_oXmlDoc = new XmlDocument();
                m_oXmlDoc.Load(m_sFilePath);
            }
            


            XmlNodeList xmlChildLst = m_oXmlDoc.ChildNodes;
            XmlNode xmlContext = null;
            foreach (XmlNode oChildNode in xmlChildLst)
            {
                if (0 == oChildNode.Name.CompareTo("strategy_bak"))
                {
                    xmlContext = oChildNode;
                    break;
                }
            }

            if (null != xmlContext)
            {

                //remove old xmlnode
                XmlNodeList xmlNodeLst = xmlContext.ChildNodes;
                
                foreach (XmlNode oXmlNode in xmlNodeLst)
                {
                    string sId = oXmlNode.Attributes["id"].Value;
                    if (string.Empty != sId && 0 == sId.CompareTo(oContext.Uid))
                    {
                        xmlContext.RemoveChild(oXmlNode);
                        break;
                    }
                }
               
                XmlElement xmlStrategy = m_oXmlDoc.CreateElement("strategy_case");
                xmlStrategy.SetAttribute("status", "0");
                xmlStrategy.SetAttribute("id", oContext.Uid);
                xmlContext.AppendChild(xmlStrategy);


                XmlElement xmlName = m_oXmlDoc.CreateElement("name");
                xmlName.InnerText = oContext.StrategyName;
                xmlStrategy.AppendChild(xmlName);

                XmlElement xmlParams = m_oXmlDoc.CreateElement("params");
                xmlStrategy.AppendChild(xmlParams);

                //param
                for (int j = 0; j < oContext.Params.Count; j++)
                {
                    XmlElement xmlParam = m_oXmlDoc.CreateElement("param");
                    xmlParam.SetAttribute("name", oContext.Params[j].ParamName);
                    xmlParam.SetAttribute("value", oContext.Params[j].ParamValue);
                    xmlParams.AppendChild(xmlParam);
                }

                //contract_postion
                XmlElement xmlContractPostion = m_oXmlDoc.CreateElement("contract_postion");
                xmlStrategy.AppendChild(xmlContractPostion);
                foreach (ContractPostion pos in oContext.Postions)
                {
                    XmlElement xmlPostion = m_oXmlDoc.CreateElement("contract");
                    xmlPostion.SetAttribute("code", pos.ContractCode);
                    xmlPostion.SetAttribute("name", pos.ContractName);
                    xmlPostion.SetAttribute("postion", pos.Postion.ToString());
                    xmlPostion.SetAttribute("open_price", pos.OpenPrice.ToString());
                    xmlPostion.SetAttribute("dir", pos.Direction.ToString());
                    xmlPostion.SetAttribute("open_time", pos.OpenTime);
                    xmlPostion.SetAttribute("high_price", pos.HighPrice.ToString());
                    xmlContractPostion.AppendChild(xmlPostion);
                }

                m_oXmlDoc.Save(m_sFilePath);
            }


        }

        public virtual List<StrategyParam> ParseParam(string sParamName, string sParamValue)
        {
            List<StrategyParam> oLstParams = new List<StrategyParam>();
           // StrategyParam oParam = null;
            if (string.Empty != sParamName && string.Empty != sParamValue)
            {
                string[] arParamNames = sParamName.Split(';');
                string[] arParamValues = sParamValue.Split(';');
                if (arParamNames.Length > 0 && arParamNames.Length == arParamValues.Length)
                {
                    for (int i = 0; i < arParamNames.Length; i++ )
                    {
                        StrategyParam oParam = new StrategyParam();
                        oParam.ParamName = arParamNames[i];
                        oParam.ParamValue = arParamValues[i];

                        oLstParams.Add(oParam);
                    }
                }
            }
            return oLstParams;
        }

        //300481;濮阳惠成;100;0.000000;0;20160428 000000$600740;山西焦化;1400;0.000000;0;20160428 000000$
        public virtual List<ContractPostion> ParsePostion(string sPostions)
        {
            List<ContractPostion> oLstPostion = new List<ContractPostion>();

            if (string.Empty != sPostions)
            {
                string[] strPos = sPostions.Split('$');

                for (int i = 0; i < strPos.Length; i++ )
                {
                    if (string.Empty == strPos[i])
                        continue;

                    string[] tempPos = strPos[i].Split(';');

                    if (tempPos.Length >= 6)
                    {
                        ContractPostion cPos = new ContractPostion();

                        cPos.ContractCode = tempPos[0];
                        cPos.ContractName = tempPos[1];
                        cPos.Postion = int.Parse(tempPos[2]);
                        cPos.OpenPrice = double.Parse(tempPos[3]);
                        cPos.Direction = int.Parse(tempPos[4]);
                        cPos.OpenTime = tempPos[5];
                        if (tempPos.Length > 6)
                        {
                            cPos.HighPrice = double.Parse(tempPos[6]);
                        }
                        

                        oLstPostion.Add(cPos);
                    }

                }
              
            
            }


            return oLstPostion;
        }


        public void RemoveStrategy(string sStrategyId)
        {
            MakesureXmlExist();
            try
            {
                m_oXmlDoc = new XmlDocument();
                m_oXmlDoc.Load(m_sFilePath);
            }
            catch (System.Exception ex)
            {
                Log.WriteLog("RemoveStrategy" + ex.ToString(), 1);
            	//xml format is not correct, remake it
                File.Delete(m_sFilePath);
                MakesureXmlExist();

                m_oXmlDoc = new XmlDocument();
                m_oXmlDoc.Load(m_sFilePath);
            }
            


            XmlNodeList xmlChildLst = m_oXmlDoc.ChildNodes;
            XmlNode xmlContext = null;
            foreach (XmlNode oChildNode in xmlChildLst)
            {
                if (0 == oChildNode.Name.CompareTo("strategy_bak"))
                {
                    xmlContext = oChildNode;
                    break;
                }
            }

            if (null != xmlContext)
            {

                //remove old xmlnode
                XmlNodeList xmlNodeLst = xmlContext.ChildNodes;

                foreach (XmlNode oXmlNode in xmlNodeLst)
                {
                    string sId = oXmlNode.Attributes["id"].Value;
                    if (string.Empty != sId && 0 == sId.CompareTo(sStrategyId))
                    {
                        xmlContext.RemoveChild(oXmlNode);
                        break;
                    }
                }
            } 
            m_oXmlDoc.Save(m_sFilePath);
        }
    }
}
