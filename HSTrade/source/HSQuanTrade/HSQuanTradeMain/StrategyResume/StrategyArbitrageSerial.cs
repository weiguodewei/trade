using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace HSQuanTradeMain.StrategyResume
{
    public class StrategyArbitrageSerial : StrategySerial
    {
        protected override void ParseStrategyContext(System.Xml.XmlNode oXmlNode, ref List<StrategyContext> oLstContext)
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
                                    
                                    oStragtegySer.Postions.Add(oPostion);
                                }
                            }

                        }

                        oLstContext.Add(oStragtegySer);
                    }
                }


            }
        }

        public override List<StrategyParam> ParseParam(string sParamName, string sParamValue)
        {
            return base.ParseParam(sParamName, sParamValue);
        }

        public override List<ContractPostion> ParsePostion(string sPostions)
        {
           List<ContractPostion> oLstPostion = new List<ContractPostion>();

           if (string.Empty != sPostions)
           {
               string[] strPos = sPostions.Split('$');

               for (int i = 0; i < strPos.Length; i++)
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
                       


                       oLstPostion.Add(cPos);
                   }

               }
           }
           return oLstPostion;
        }

        public override void Save(StrategyContext oContext)
        {
            MakesureXmlExist();
            try
            {
                m_oXmlDoc = new XmlDocument();
                m_oXmlDoc.Load(m_sFilePath);
            }
            catch (System.Exception ex)
            {
                Log.WriteLog("Save" + ex.ToString(), 1);
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
                    xmlContractPostion.AppendChild(xmlPostion);
                }

                m_oXmlDoc.Save(m_sFilePath);
            }
        }
    }
}
