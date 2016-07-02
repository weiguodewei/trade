using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HSQuanTradeMain
{
    public class StrategryManager
    {
        public List<string> listmodelrun = new List<string>();

        public delegate_GetStrategyStatusInfo getstrategystatus = null;

        public delegate_getstrategyparams getstrategyparams = null;

        public delegate_insertrecord insertrecord = null;

        public delete_getValues getValues = null;

        public delegate_SetStrategyParam setValues = null;

        public delete_getSignal getSignal = null;

        public delete_stop stop = null;

        public delete_run run = null;

        public delete_pause pause = null;


        public void addmodel(string modelname)
        {
            listmodelrun.Add(modelname);

            insertmodeltolist(modelname);
        }


        /// <summary>
        /// 插入一条记录到列表
        /// </summary>
        /// <param name="modelname"></param>
        public void insertmodeltolist(string modelname)
        {
            string status = getstrategystatus(modelname);

            if (insertrecord != null)
            {
                insertrecord(modelname,status);
            }


        }

        //根据name获取策略的参数值
        public string[] getParamsByModelName(string name)
        {

            string s = getstrategyparams(name);

            string[] p = s.Split(';');

            return p;
        }

        //根据modelname获取model的参数值
        public string[] getValueStr(string modelname, string param)
        {
            string s = getValues(modelname, param);

            string[] p = s.Split(';');

            return p;
        
        }

        //设置model的参数值
        public void setValueStr(string modelname, string paramname, string value)
        {
            setValues(modelname, paramname, value);
        
        }

    };
}
