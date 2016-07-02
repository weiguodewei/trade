using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Office.Interop.Excel;
using System.Data;
using System.Collections;

namespace HSQuanTradeMain
{
    class ExcelUtil
    {

        private Stopwatch sw = new Stopwatch();

        public System.Data.DataTable GetExcelData(string path)
        {
            Application app = new Application();
            Workbook workbook = null;
            Sheets sheets;
            object oMissiong = System.Reflection.Missing.Value;
            System.Data.DataTable dt = new System.Data.DataTable();

            sw.Start();

            try
            {
                if (app == null)
                {
                    return null;
                }

                workbook = app.Workbooks.Open(path, oMissiong, oMissiong, oMissiong, oMissiong, oMissiong, oMissiong, oMissiong, oMissiong, oMissiong, oMissiong, oMissiong, oMissiong, oMissiong, oMissiong);

                //将数据读入到DataTable中——Start   

                sheets = workbook.Worksheets;
                Worksheet worksheet = (Worksheet)sheets.get_Item(1);//读取第一张表

                //Worksheet worksheet = (Worksheet)sheets["交易计划"];              

                if (worksheet == null)
                    return null;

                string cellContent;
                //总行数 包括标题
                int iRowCount = worksheet.UsedRange.Rows.Count;
                int iColCount = worksheet.UsedRange.Columns.Count;
                //iColCount = 7;
                Range range;

                //负责列头Start
                DataColumn dc;
                int ColumnID = 1;
                range = (Range)worksheet.Cells[1, 1];
                //worksheet.Cells.get_Range()
                while (range.Text.ToString().Trim() != "")
                {
                    dc = new DataColumn();
                    dc.DataType = System.Type.GetType("System.String");
                    dc.ColumnName = range.Text.ToString().Trim();
                    dt.Columns.Add(dc);

                    range = (Range)worksheet.Cells[1, ++ColumnID];
                }
                //End

                for (int iRow = 2; iRow <= iRowCount; iRow++)
                {
                    DataRow dr = dt.NewRow();

                    for (int iCol = 1; iCol <= iColCount; iCol++)
                    {
                        range = (Range)worksheet.Cells[iRow, iCol];

                        cellContent = (range.Value2 == null) ? "" : range.Text.ToString();

                        //if (iRow == 1)
                        //{
                        //    dt.Columns.Add(cellContent);
                        //}
                        //else
                        //{
                        dr[iCol - 1] = cellContent;
                        //}
                    }

                    //if (iRow != 1)
                    dt.Rows.Add(dr);
                }

                sw.Stop();
                TimeSpan ts = sw.Elapsed;

                //将数据读入到DataTable中——End
                return dt;
            }
            catch
            {

                return null;
            }
            finally
            {
                workbook.Close(false, oMissiong, oMissiong);
                System.Runtime.InteropServices.Marshal.ReleaseComObject(workbook);
                workbook = null;
                app.Workbooks.Close();
                app.Quit();
                System.Runtime.InteropServices.Marshal.ReleaseComObject(app);
                app = null;

                Process[] procs = Process.GetProcessesByName("EXCEL");
                foreach (Process pro in procs)
                {
                    pro.Kill();//没有更好的方法,只有杀掉进程
                }

                GC.Collect();
                GC.WaitForPendingFinalizers();
            }


        
        }


        public System.Data.DataTable GetTradeData(string path, string strdate)
        {

            Application app = new Application();
            Workbook workbook = null;
            Sheets sheets;
            object oMissiong = System.Reflection.Missing.Value;
            System.Data.DataTable dt = new System.Data.DataTable();

            app.DisplayAlerts = false;

            sw.Start();

            try
            {
                if (app == null)
                {
                    return null;
                }

                workbook = app.Workbooks.Open(path, oMissiong, oMissiong, oMissiong, oMissiong, oMissiong, oMissiong, oMissiong, oMissiong, oMissiong, oMissiong, oMissiong, oMissiong, oMissiong, oMissiong);

                //将数据读入到DataTable中——Start   

                sheets = workbook.Worksheets;
                //Worksheet worksheet = (Worksheet)sheets.get_Item(3);//读取第一张表

                Worksheet worksheet = (Worksheet)sheets["交易计划"];

                if (worksheet == null)
                    return null;

                int index = -1;

                string cellContent;
                //总行数 包括标题
                int iRowCount = worksheet.UsedRange.Rows.Count;
                int iColCount = worksheet.UsedRange.Columns.Count;
                iColCount = 8;
                Range range;

                //负责列头Start
                DataColumn dc;
                int ColumnID = 1;
                range = (Range)worksheet.Cells[1, 1];
                //worksheet.Cells.get_Range()
                while (range.Text.ToString().Trim() != "")
                {
                    dc = new DataColumn();
                    dc.DataType = System.Type.GetType("System.String");
                    dc.ColumnName = range.Text.ToString().Trim();
                    dt.Columns.Add(dc);

                    range = (Range)worksheet.Cells[1, ++ColumnID];
                }
                //End

                for (int iRow = 2; iRow <= iRowCount; iRow++)
                {
                    range = (Range)worksheet.Cells[iRow, 1];

                    cellContent = (range.Value2 == null) ? "" : range.Text.ToString();

                    string [] str = cellContent.Split('/');

                    string month = str[1];
                    string day = str[2];

                    if (day.Length == 1)
                    {
                        day = "0" + day;
                    }

                    if (month.Length == 1)
                    {
                        month = "0" + month;
                    }

                    string strxxx = string.Concat(month, day);

                    if (strdate.IndexOf(strxxx)>0)
                    {
                        index = iRow;
                        break;
                    }
                
                }

                for (int iRow = index; iRow <= iRowCount; iRow++)
                {
                     DataRow dr = dt.NewRow();

                     for (int iCol = 1; iCol <= iColCount; iCol++)
                     {
                         range = (Range)worksheet.Cells[iRow, iCol];

                         cellContent = (range.Value2 == null) ? "" : range.Text.ToString();

                         dr[iCol - 1] = cellContent;

                     }

                     dt.Rows.Add(dr);
                 }

                sw.Stop();
                TimeSpan ts = sw.Elapsed;

                //将数据读入到DataTable中——End
                return dt;

            }
            catch
            {

                return null;
            }
            finally
            {
                workbook.Close(false, oMissiong, oMissiong);
                System.Runtime.InteropServices.Marshal.ReleaseComObject(workbook);
                workbook = null;
                app.Workbooks.Close();
                app.Quit();
                System.Runtime.InteropServices.Marshal.ReleaseComObject(app);
                app = null;

                //Process[] procs = Process.GetProcessesByName("EXCEL");
                //foreach (Process pro in procs)
                //{
                //    pro.Kill();//没有更好的方法,只有杀掉进程
                //}

                GC.Collect();
                GC.WaitForPendingFinalizers();
            }

        }




    }
}
