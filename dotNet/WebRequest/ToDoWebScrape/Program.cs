// https://docs.microsoft.com/en-us/dotnet/framework/network-programming/how-to-request-data-using-the-webrequest-class

//#define DEBUG_A_TABLE

using HtmlAgilityPack;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Net;
using System.Reflection;
using System.Text.RegularExpressions;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Web;
using Microsoft.VisualBasic;

namespace ToDoWebScrape
{
    // In a day, there are several races, each race has its own id, in the format YYYY-raceId,         
    //                                                                                                 
    public class RacingDate
    {
        public DateTime m_dateTime;
        //public enum LocalType {
        //    Unknown, 
        //    Local, 
        //    Simulcast
        //}
        public String m_localType = String.Empty; 
        public String m_field = String.Empty;//ST 沙田, HV 跑马地, 
        
        // key: pageId for example 1/2/3...; value: href link;
        public SortedList<Int32, String> m_aPageIdToLink = new SortedList<int, string>();

        public static Int32 DateTimeToInt(DateTime dt) { return dt.Year * 10000 + dt.Month * 100 + dt.Day; }
        public enum ColumnName : UInt32
        {
            Date = 0, 
            Local = 1, 
            Field = 2, 
            PageId = 3, 
            Link = 4,
            Count  
        }
        public static Int32 readDateFromFile(SortedList<Int32, RacingDate> aRacingDate, String sDatePageIdLinkFile)
        {
            Int32 iReadRowCount = 0;
            try
            {
                using (Mono.Csv.CsvFileReader reader = new Mono.Csv.CsvFileReader(sDatePageIdLinkFile))
                {
                    List<List<string>> dataGrid = new List<List<string>>();
                    reader.ReadAll(dataGrid);

                    if (dataGrid.Count >= 1)
                    {
                        int iFirstRowDate = 0;
                        if (int.TryParse(dataGrid[0][0], out iFirstRowDate))
                        {
                        }
                        int iStartIdx = 0;
                        if (iFirstRowDate == 0)
                        {
                            // there is header
                            iStartIdx = 1;
                        }
                        else
                        {
                            // there is no header 
                        }
                        for (; iStartIdx < dataGrid.Count; ++iStartIdx)
                        {
                            List<String> columns = dataGrid[iStartIdx];
                            int iDate = 0;
                            if (int.TryParse(columns[(Int32)ColumnName.Date], out iDate))
                            {
                                // a valid row 
                                if (aRacingDate.ContainsKey(iDate))
                                {
                                    aRacingDate[iDate].m_aPageIdToLink.Add(int.Parse(columns[(Int32)ColumnName.PageId]), columns[(Int32)ColumnName.Link]);
                                }
                                else
                                {
                                    RacingDate rd = new RacingDate();
                                    rd.m_dateTime = DateTime.ParseExact(iDate.ToString(), "yyyyMMdd", System.Globalization.CultureInfo.InvariantCulture);
                                    rd.m_localType = columns[(Int32)ColumnName.Local];
                                    rd.m_field = columns[(Int32)ColumnName.Field];
                                    rd.m_aPageIdToLink.Add(int.Parse(columns[(Int32)ColumnName.PageId]), columns[(Int32)ColumnName.Link]);

                                    aRacingDate.Add(iDate, rd);
                                }
                                ++iReadRowCount;
                            }
                        } // end of for each row
                    }
                }
            }
            catch (FileNotFoundException)
            {
                Console.WriteLine($@"Find not found, {sDatePageIdLinkFile}.");
            }
            return iReadRowCount;
        }
        public static Int32 writeDateToFile(SortedList<Int32, RacingDate> aRacingDate, String sDatePageIdLinkFile)
        {
            int iRowCountWriteToFile = 0;
            using (Mono.Csv.CsvFileWriter writer = new Mono.Csv.CsvFileWriter(sDatePageIdLinkFile))
            {
                foreach (KeyValuePair<Int32, RacingDate> kvp in aRacingDate)
                {
                    Int32 iDate = kvp.Key;
                    RacingDate rd = kvp.Value;

                    foreach (KeyValuePair<Int32, String> kvp2 in rd.m_aPageIdToLink)
                    {
                        List<String> columns = new List<string>();
                        for (int i = 0; i < (Int32)ColumnName.Count; ++i)
                            columns.Add(String.Empty);
                        columns[(Int32)ColumnName.Date] = (Convert.ToString(iDate));
                        columns[(Int32)ColumnName.Local] = (rd.m_localType);
                        columns[(Int32)ColumnName.Field] = (rd.m_field);
                        columns[(Int32)ColumnName.PageId] = (Convert.ToString(kvp2.Key));
                        columns[(Int32)ColumnName.Link] = (kvp2.Value);

                        ++ iRowCountWriteToFile;
                        writer.WriteRow(columns);
                    }
                }
            }
            return iRowCountWriteToFile;
        }
    }
    public class RaceId
    {
        //public Int32 m_raceId = -1; // explicit this will make this infor much more reduction.
        public Int64 m_iYearUid = -1;
        public RaceId() { }  
        public RaceId(String sYearUid)
        {
            String[] set = Regex.Split(sYearUid, "-");
            Int32 iYear = Convert.ToInt32(set[0]);
            Int32 iUid = Convert.ToInt32(set[1]);
            m_iYearUid = RaceId.YearRaceId(iYear, iUid);
        }          
        public DateTime Date()
        {
            Int32 a = (Int32)(m_iYearUid / 10000);
            return DateTime.ParseExact(a.ToString(), "yyyyMMdd", System.Globalization.CultureInfo.InvariantCulture);
        }
        public Int32 Uid()
        {
            Int32 a = (Int32)(m_iYearUid % 1000);
            return a;
        }
        // integer format YYYYUID, 20160001, 20160002, ... i think int comparison is faster than string comparison. 
        // string  format YYYY-UID,2016-1,2016-2,...
        public static Int64 YearRaceId(Int32 iYear, Int32 iRaceId)
        {
            return iYear * (Int64)10000 + iRaceId;
        }
        public static Int64 YearRaceId(DateTime date, Int32 iRaceId)
        {
            Int32 a = RacingDate.DateTimeToInt(date);
            Int64 b = a * (Int64)10000;
            Int64 c = b + iRaceId;
            return  c;
        }
        public static Int64 YearRaceId(String sYear, String sRaceId)
        {
            return YearRaceId(Convert.ToInt32(sYear), Convert.ToInt32(sRaceId));
        }
        public static Int64 YearRaceId(DateTime date, String sRaceId)
        {
            return YearRaceId(RacingDate.DateTimeToInt(date), Convert.ToInt32(sRaceId));
        }
        public static String YearRaceIdToString(Int64 iYearUid)
        {
            Int64 a = iYearUid / 10000;
            Int64 b = iYearUid % 10000;
            return (a).ToString() + "-" + (b);
        }
    }
    public class RaceIdMapDatePageId
    {
        public RaceId m_raceId = new RaceId();
        public DateTime m_aDate = new DateTime();
        public Int32 m_iPageId = -1;
        public static Int32 readRowsFromFile(SortedList<Int64, RaceIdMapDatePageId> aRaceIdMapDatePageId, String sFile)
        {
            Int32 iReadRowCount = 0;
            try
            {
                using (Mono.Csv.CsvFileReader reader = new Mono.Csv.CsvFileReader(sFile))
                {
                    List<List<string>> dataGrid = new List<List<string>>();
                    reader.ReadAll(dataGrid);

                    if (dataGrid.Count >= 1)
                    {
                        Int64 iFirstRowDate = 0;
                        if (Int64.TryParse(dataGrid[0][0], out iFirstRowDate))
                        {
                        }
                        int iStartIdx = 0;
                        if (iFirstRowDate == 0)
                        {
                            // there is header
                            iStartIdx = 1;
                        }
                        else
                        {
                            // there is no header 
                        }
                        for (; iStartIdx < dataGrid.Count; ++iStartIdx)
                        {
                            List<String> columns = dataGrid[iStartIdx];
                            Int64 iYearUid = 0;
                            if (Int64.TryParse(columns[0], out iYearUid))
                            {
                                // a valid row 
                                if (aRaceIdMapDatePageId.ContainsKey(iYearUid))
                                {
                                }
                                else
                                {
                                    RaceIdMapDatePageId rd = new RaceIdMapDatePageId();
                                    rd.m_raceId.m_iYearUid = iYearUid; ; 
                                    String[] set = Regex.Split(columns[1], "-");
                                    Debug.Assert(rd.m_raceId.m_iYearUid == RaceId.YearRaceId(set[0], set[1]));
                                    rd.m_aDate = DateTime.ParseExact(columns[2], "yyyyMMdd", System.Globalization.CultureInfo.InvariantCulture); ;
                                    rd.m_iPageId = Convert.ToInt32(columns[3]);

                                    aRaceIdMapDatePageId.Add(iYearUid, rd);
                                }
                                ++iReadRowCount;
                            }
                        } // end of for each row
                    }
                }
            }
            catch (FileNotFoundException)
            {
                Console.WriteLine($@"File not found {sFile}");
            }
            return iReadRowCount;
        }
        public static Int32 writeRowsToFile(SortedList<Int64, RaceIdMapDatePageId> aRaceIdMapDatePageId, String sFile)
        {
            int iRowCountWriteToFile = 0;
            using (Mono.Csv.CsvFileWriter writer = new Mono.Csv.CsvFileWriter(sFile))
            {
                foreach (KeyValuePair<Int64, RaceIdMapDatePageId> kvp in aRaceIdMapDatePageId)
                {
                    Int64 iYearUid = kvp.Key;
                    RaceIdMapDatePageId rd = kvp.Value; 
                    {
                        List<String> columns = new List<string>();
                        for (int i = 0; i < 4; ++i)
                            columns.Add(String.Empty);
                        columns[0] = rd.m_raceId.m_iYearUid.ToString();
                        columns[1] = RaceId.YearRaceIdToString(rd.m_raceId.m_iYearUid);
                        Debug.Assert(iYearUid == rd.m_raceId.m_iYearUid);
                        columns[2] = rd.m_aDate.ToString("yyyyMMdd");
                        columns[3] = rd.m_iPageId.ToString(); 

                        ++iRowCountWriteToFile;
                        writer.WriteRow(columns);
                    }
                }
            }
            return iRowCountWriteToFile;
        }

    }
    // race result table
    public class RRTable
    {
        public DateTime m_iDate = new DateTime();
        public Int32 m_iUid = -1;
        public List<List<String>> m_rows = new List<List<string>>();
        
        public static void ReadRaceResultFromFile(SortedList<Int64, RRTable> aRaceIdToResultTable, String sFile)
        {
            try
            {
                using (Mono.Csv.CsvFileReader reader = new Mono.Csv.CsvFileReader(sFile))
                {
                    List<List<string>> dataGrid = new List<List<string>>();
                    reader.ReadAll(dataGrid);

                    if (dataGrid.Count >= 1)
                    {
                        int iFirstRowFirstColumn = 0;
                        if (int.TryParse(dataGrid[0][0], out iFirstRowFirstColumn))
                        {
                        }
                        int iStartIdx = 0;
                        if (iFirstRowFirstColumn == 0)
                        {
                            // there is header
                            iStartIdx = 1;
                        }
                        else
                        {
                            // there is no header 
                        }
                        String sLastYearUid = String.Empty;
                        Int64 iLastYearUid = -1;
                        RRTable lastTable = new RRTable();
                        for (; iStartIdx < dataGrid.Count; ++iStartIdx)
                        {
                            List<String> newColumns = dataGrid[iStartIdx];
                            // a valid row  
                            if (newColumns[18] != sLastYearUid)
                            {
                                // this row will start a new uid, new race table.
                                if (iLastYearUid != -1)
                                {
                                    aRaceIdToResultTable.Add(iLastYearUid, lastTable);
                                }
                                sLastYearUid = newColumns[18];
                                RaceId rid = new RaceId(sLastYearUid);
                                iLastYearUid = rid.m_iYearUid;
                                lastTable = new RRTable();
                                lastTable.m_iDate = rid.Date();
                                lastTable.m_iUid = rid.Uid();
                                int a = 0;
                                int b = a;
                                ++b;
                            }
                            else
                            {
                                // this row still belong to the last race table.
                            }
                            List<String> oldColumns = new List<String>();
                            for (int i = 0; i < 18; ++i)
                                oldColumns.Add(String.Empty);
                            oldColumns[0] = newColumns[0]; // finishing num 
                            oldColumns[1] = newColumns[1]; // horse num 
                            oldColumns[2] = newColumns[2]; // horse name 
                            oldColumns[3] = newColumns[4]; // jockey
                            oldColumns[4] = newColumns[5]; // trainer
                            oldColumns[5] = newColumns[6]; // actual weight
                            oldColumns[6] = newColumns[7]; // declare weight
                            oldColumns[7] = newColumns[8]; // draw 档位
                            oldColumns[8] = newColumns[9]; // length behind win 
                            oldColumns[9] = String.Empty;
                            oldColumns[10] = newColumns[14]; // finish time 
                            oldColumns[11] = newColumns[15]; // win odds
                            oldColumns[12] = newColumns[10] == "NA" ? String.Empty : newColumns[10];
                            oldColumns[13] = newColumns[11] == "NA" ? String.Empty : newColumns[11];
                            oldColumns[14] = newColumns[12] == "NA" ? String.Empty : newColumns[12];
                            oldColumns[15] = newColumns[13] == "NA" ? String.Empty : newColumns[13];
                            oldColumns[16] = newColumns[16] == "NA" ? String.Empty : newColumns[16];
                            oldColumns[17] = newColumns[17] == "NA" ? String.Empty : newColumns[17];
                            lastTable.m_rows.Add(oldColumns);
                        } // end of for each row
                    }
                }
            }
            catch (FileNotFoundException)
            {
                Console.WriteLine($@"Find not found {sFile}");
            }
        }
        public static Int32 WriteRaceResultToCSVFile(SortedList<Int64, RRTable> aRaceIdToResultTable, String sFile)
        {
            Int32 iRaceIdWroteCount = 0;
            using (Mono.Csv.CsvFileWriter writer = new Mono.Csv.CsvFileWriter(sFile))
            {
                {
                    foreach (var raceIdToResult in aRaceIdToResultTable)
                    {
                        RRTable table = raceIdToResult.Value;
                        if (table.m_iUid == -1)
                            continue;
                        ++iRaceIdWroteCount;
        
                        for (int ir = 0; ir < table.m_rows.Count; ++ir)
                        {
                            List<String> oldColumns = table.m_rows[ir];// one row
                            List<String> newColumns = new List<String>();
                            for (int i = 0; i < 19; ++i)
                                newColumns.Add(String.Empty);
                            newColumns[0] = oldColumns[0]; // finishing num
                            newColumns[1] = oldColumns[1]; // horse num
                            String sHorseName = oldColumns[2];
                            newColumns[2] = oldColumns[2]; // horse name
                            String sHorseId = sHorseName.Substring(sHorseName.LastIndexOf("(") + 1, sHorseName.LastIndexOf(")") - sHorseName.LastIndexOf("(") - 1);
                            newColumns[3] = sHorseId; // horse id
                            newColumns[4] = oldColumns[3]; // jockey 
                            newColumns[5] = oldColumns[4]; // trainer
                            newColumns[6] = oldColumns[5]; // actual weight
                            newColumns[7] = oldColumns[6]; // declare weight 
                            newColumns[8] = oldColumns[7]; // draw 档位
                            newColumns[9] = oldColumns[8]; // length behind win
                            Int32 iRunPosCount = oldColumns.Count - 12; // hkjc only has 12 column in a row.
                            newColumns[10] = ((iRunPosCount > 0 && oldColumns.Count > 12) ? oldColumns[12] : "NA"); // running position 1
                            newColumns[11] = ((iRunPosCount > 1 && oldColumns.Count > 13) ? oldColumns[13] : "NA"); // running position 2
                            newColumns[12] = ((iRunPosCount > 2 && oldColumns.Count > 14) ? oldColumns[14] : "NA"); // running position 3
                            newColumns[13] = ((iRunPosCount > 3 && oldColumns.Count > 15) ? oldColumns[15] : "NA"); // running position 4
                            newColumns[14] = oldColumns[10]; // finish time 
                            newColumns[15] = oldColumns[11]; // win odds
                            newColumns[16] = ((iRunPosCount > 4 && oldColumns.Count > 16) ? oldColumns[16] : "NA"); // running position 5
                            newColumns[17] = ((iRunPosCount > 5 && oldColumns.Count > 17) ? oldColumns[17] : "NA"); // running position 6
                            newColumns[18] = RaceId.YearRaceIdToString(RaceId.YearRaceId(table.m_iDate, table.m_iUid)); // race id

                            writer.WriteRow(newColumns);
                        }
                        //Console.WriteLine("  End of write result of race id " + sRaceId);
                    }
                }
            }
            return iRaceIdWroteCount;
        }

    }

    public class WebRequestGetExample
    {
        public static String testWebRequest(String sLink)
        {// Create a request for the URL.   
            WebRequest request = WebRequest.Create( 
                sLink
              );
            // If required by the server, set the credentials.  
            request.Credentials = CredentialCache.DefaultCredentials;
            // Get the response.  
            WebResponse response = request.GetResponse();
            // Display the status.  
            //Console.WriteLine(((HttpWebResponse)response).StatusDescription);
            // Get the stream containing content returned by the server.  
            Stream dataStream = response.GetResponseStream();
            // Open the stream using a StreamReader for easy access.  
            StreamReader reader = new StreamReader(dataStream);
            // Read the content.  
            string responseFromServer = reader.ReadToEnd();
            // Display the content.  
            //Console.WriteLine(responseFromServer);
            // Clean up the streams and the response.  
            reader.Close();
            response.Close();
            return responseFromServer;
            {
                // to do the same thing. 
                // https://docs.microsoft.com/en-us/dotnet/framework/network-programming/how-to-request-a-web-page-and-retrieve-the-results-as-a-stream

                //WebClient myClient = new WebClient();
                //Stream response2 = myClient.OpenRead("https://cn.bing.com");
                //// The stream data is used here.   
                //response2.Close();
            }
        }
        public static void GetRacingDates(String sLink, SortedList<Int32, RacingDate> aRacingDate)
        { 
            HtmlWeb htmlWeb = new HtmlWeb();
            HtmlDocument htmlDoc = htmlWeb.Load(
                sLink
                );

            HtmlNodeCollection selectCol = htmlDoc.DocumentNode.SelectNodes("//select");
            if (selectCol == null)
                return;
            foreach (HtmlNode select in selectCol)
            {
                // to use this specify div to find out all the links.
                if (select.GetAttributeValue("name", String.Empty) == "raceDateSelect")
                {
                    // get how many racing at this day.
                    String sSelectInnerText = select.InnerText;
                    String sSelectInnerHtml = select.InnerHtml;

                    List<HtmlNode> optionList = select.Elements("option").ToList();
                    for (int iO = 0; iO < optionList.Count; ++iO)
                    {
                        HtmlNode optionNode = optionList[iO];
                        String sValue = optionNode.GetAttributeValue("value", String.Empty);
                        if (sValue == String.Empty)
                            continue;
                        String[] sets = Regex.Split(sValue, "/");
                        RacingDate racingDate = new RacingDate();
                        racingDate.m_dateTime = DateTime.ParseExact(sets[1], "yyyyMMdd", System.Globalization.CultureInfo.InvariantCulture);
                        racingDate.m_localType = sets[0];
                        racingDate.m_field = sets[2];

                        Int32 iDateTime = Convert.ToInt32(sets[1]);
                        aRacingDate.Add(iDateTime, racingDate);
                    }
                }
            }
        }
        public static void GetPageIdAndLink(String sLink, SortedList<Int32, String> aPageIdToLink)
        {
            String[] sSets = Regex.Split(sLink, "/");
            Int32 iCurrentPageId = Convert.ToInt32(sSets.Last());
      
            if (aPageIdToLink.ContainsKey(iCurrentPageId))
            {
                Debug.Assert(sLink == aPageIdToLink[iCurrentPageId]);
            }
            else 
                aPageIdToLink.Add(iCurrentPageId, sLink);
            bool bChinese = sLink.IndexOf("chinese", StringComparison.OrdinalIgnoreCase) >= 0;
            if (!bChinese)
            {
                Debug.Assert(sLink.IndexOf("english", StringComparison.OrdinalIgnoreCase) >=0);
            }
            bool bLocal = sLink.IndexOf("Local", StringComparison.OrdinalIgnoreCase) >= 0;

            //HtmlWeb htmlWeb = new HtmlWeb();
            //HtmlDocument htmlDoc = htmlWeb.Load(
            //    sCurrentLink
            //    );
            String sHtmlContest = testWebRequest(sLink);
            int iTryCount = 0;
            while (iTryCount++ < 100)
            {
                if (sHtmlContest.IndexOf("raceNum clearfix", StringComparison.OrdinalIgnoreCase) >= 0)
                {
                    break;
                }
            }
            HtmlDocument htmlDoc = new HtmlDocument();
            htmlDoc.LoadHtml(sHtmlContest);
            HtmlNodeCollection divCol = htmlDoc.DocumentNode.SelectNodes("//div");
            foreach (HtmlNode div in divCol)
            {
                // to use this specify div to find out all the links.
                String sAttValue = div.GetAttributeValue("class", String.Empty);
                if (sAttValue.IndexOf("raceNum clearfix", StringComparison.OrdinalIgnoreCase) >= 0)
                {
                    // get how many racing at this day.
                    String sInnerText = div.InnerText;
                    String sInnerHtml = div.InnerHtml;
                    HtmlNodeCollection tdCol = div.SelectNodes(".//td");
                    Int32 iTdCount = tdCol.Count; // 去掉开头的2个(&nbsp,沙田), 最后的一个"所有场次"

                    for (int i = 2; i < iTdCount - 1; ++i)
                    {
                        HtmlNode td = tdCol[i];
                        HtmlNode tdFirstChild = td.Descendants().First();
                        //< a href = "/racing/info/meeting/Results/chinese/Local/20170604/ST/6" >< img src = "/racing/info/StaticFile/Images/Racing/racecard_rt_6.gif" name = "racecard_rt_6_r" id = "racecard_rt_6_r" border = "0" ></ a >
                        String sHref = tdFirstChild.GetAttributeValue("href", String.Empty);
                        if (sHref == String.Empty)
                        { 
                        }
                        else
                        {
                            // to find out valid link 
                            if (sHref.IndexOf("Results", StringComparison.OrdinalIgnoreCase) >= 0 
                                && sHref.IndexOf("ResultsAll", StringComparison.OrdinalIgnoreCase) < 0 // 排除 所有场次
                                )
                            {
                                //bChinese == sHref.IndexOf("Chinese", StringComparison.OrdinalIgnoreCase) >=0 && 
                                if (    bLocal == sHref.IndexOf("Local", StringComparison.OrdinalIgnoreCase)>=0)
                                { 
                                    Int32 iExtractedLastPageId = Convert.ToInt32(Regex.Split(sHref, "/").Last());
                                    if (aPageIdToLink.ContainsKey(iExtractedLastPageId))
                                    {
                                        Debug.Assert(aPageIdToLink[iExtractedLastPageId].EndsWith(sHref, StringComparison.OrdinalIgnoreCase) );
                                    }
                                    else
                                        aPageIdToLink.Add(iExtractedLastPageId, sHref);
                                }
                            }
                        }
                        int a = 0;
                        int b = a; ++b;
                    }
                }
            }
            // start from the page id 1. No page id 0!.
            for (int i = 1; i <= aPageIdToLink.Count; ++i)
            {
                if (i != iCurrentPageId)
                {
                    if (aPageIdToLink[i].StartsWith("http://racing.hkjc.com", StringComparison.OrdinalIgnoreCase))
                    {
                    }
                    else 
                        aPageIdToLink[i] = "http://racing.hkjc.com" + aPageIdToLink[i];
                }
            }
            if (aPageIdToLink.Count < 2)
            {
                //Console.WriteLine(sHtmlContest);
                int a = 0;
                int b = a;
                ++b;
            }
        }
        public static void GetResultTableFromLink(String sCurrentLink, RRTable rrTable)
        {
            bool bChinese = sCurrentLink.IndexOf("chinese", StringComparison.OrdinalIgnoreCase) >= 0;
            if (!bChinese)
            {
                Debug.Assert(sCurrentLink.IndexOf("english", StringComparison.OrdinalIgnoreCase) >= 0);
            }
            bool bLocal = sCurrentLink.IndexOf("Local", StringComparison.OrdinalIgnoreCase) >= 0;


            HtmlWeb htmlWeb = new HtmlWeb();
            HtmlDocument htmlDoc = htmlWeb.Load(
                sCurrentLink
                );

            HtmlNodeCollection divCol = htmlDoc.DocumentNode.SelectNodes("//div");
            foreach (HtmlNode div in divCol)
            {
                String sInnerText = div.InnerText;
                String sInnerHtml = div.InnerHtml;
                if (bChinese)
                {
                    if (sInnerText.Length < 50 && sInnerText.Contains("第") && sInnerText.Contains("場"))
                    {
                        // to find the race id, 457 from '第 11 場 (457)'.
                        Int32 iP0 = sInnerText.LastIndexOf('(');
                        Int32 iP1 = sInnerText.LastIndexOf(')');
                        String sId = sInnerText.Substring(iP0 + 1, iP1 - iP0 - 1);
                        rrTable.m_iUid = Convert.ToInt32(sId);
                        break;
                    }
                }
                else
                {
                    String sAttValue = div.GetAttributeValue("class", String.Empty);
                    if (sAttValue.IndexOf("boldFont14 color_white trBgBlue", StringComparison.OrdinalIgnoreCase) >= 0)
                    {
                        Int32 iP0 = sInnerText.LastIndexOf('(');
                        Int32 iP1 = sInnerText.LastIndexOf(')');
                        String sId = sInnerText.Substring(iP0 + 1, iP1 - iP0 - 1);
                        rrTable.m_iUid = Convert.ToInt32(sId);
                        break;
                    }
                }
            }
            // //table means table in the whole doc, including inner table under a table.
            HtmlNodeCollection tableArr = htmlDoc.DocumentNode.SelectNodes("//table");
            Int32 iTableCount = tableArr.Count;
#if DEBUG_A_TABLE
            Console.WriteLine($"Found tables count {iTableCount}");
#endif 
            foreach (HtmlNode table in tableArr)
            {
#if DEBUG_A_TABLE
                Console.WriteLine("Table id: " + table.Id);
#endif 
                String sTable = table.ToString();
                String sTableInnerHtml = table.InnerHtml;
                String sTableInnerText = table.InnerText;
                Int32 iTheadDescCount = table.Descendants("thead").Count();
                Int32 iTbodyDescCount = table.Descendants("tbody").Count();
#if DEBUG_A_TABLE
                Console.WriteLine($"descendant head count {iTheadDescCount}, body count {iTbodyDescCount}.");
#endif 
                if (iTheadDescCount == 1 && iTbodyDescCount == 1)
                {
                    // this should be the result table details.
                    HtmlNode theadNode = table.Descendants("thead").First();
                    if (theadNode != null)
                    {

                    }
                    HtmlNode tbodyNode = table.Descendants("tbody").First();
                    if (tbodyNode != null)
                    {
                        List<HtmlNode> trCol0 = tbodyNode.Descendants("tr").ToList();
                        List<HtmlNode> trCol = tbodyNode.Elements("tr").ToList();
                        // these two are diff, since 这个tr下面"沿途走位"下还有一个tr, 但是我们现在要最近的层.
                        for (int i = 0; i < trCol.Count; ++i)
                        {
                            // in one row, 
                            List<String> aRow = new List<String>(); 
                            HtmlNode tr = trCol[i]; // one row.
                            List<HtmlNode> tdCol0 = tr.Descendants("td").ToList();//"td", 底下几层都算
                            List<HtmlNode> tdCol = tr.Elements("td").ToList();//"td"      只算底下1层
                            if (tdCol.Count >= 12)
                            {
                                // 12 column is decided by this kind of table, 
                                List<HtmlNode> tdRunningPositions = new List<HtmlNode>();
                                for (int j = 0; j < 12; ++j)
                                {
                                    HtmlNode td = tdCol[j];
                                    //Console.WriteLine("result table cell idx " + j + ": " + td.InnerText);
                                    aRow.Add(td.InnerText);
                                    if (j <= 8)
                                    { 
                                    }
                                    else if (j == 9)
                                    {
                                        // 沿途走位  
                                        tdRunningPositions = td.Descendants("td").ToList();
                                    }
                                    else if (j == 10)
                                    {
                                        // 完成时间 
                                    }
                                    else if (j == 11)
                                    {
                                        // 独赢赔率 
                                    }
                                }
                                foreach (var a in tdRunningPositions)
                                {
                                    aRow.Add(a.InnerText);
                                }
                            }
                            rrTable.m_rows.Add(aRow);
                        }
                    }
                }
                HtmlNodeCollection theadArr = table.SelectNodes("./thead");
                Int32 iTheadLength = 0;
                if (theadArr != null) { iTheadLength = theadArr.Count; }
                HtmlNodeCollection tbodyArr = table.SelectNodes("./tbody");
                Int32 iTbodyLength = 0;
                if (tbodyArr != null) { iTbodyLength = tbodyArr.Count; }
                HtmlNodeCollection trArr = table.SelectNodes("tr");
                Int32 iTrLength = 0;
                if (trArr != null) { iTrLength = trArr.Count; }
                //Console.WriteLine($"thead count: {iTheadLength}, tbody count: {iTbodyLength}, tr count: {iTrLength}.");
                if (iTrLength > 0)
                {
                    foreach (HtmlNode row in trArr)
                    {
                        //Console.WriteLine("row");
                        foreach (HtmlNode cell in row.SelectNodes("th|td"))
                        {
                            //Console.WriteLine("cell: " + cell.InnerText);
                            String sInnerText = cell.InnerText;
                            String sInnerHtml = cell.InnerHtml;

                            if (sInnerText.Contains("賽事日期") || sInnerHtml.Contains("賽事日期")
                                || sInnerText.IndexOf("race meeting", StringComparison.OrdinalIgnoreCase) >= 0
                                || sInnerHtml.IndexOf("race meeting", StringComparison.OrdinalIgnoreCase) >= 0
                                )
                            {
                                String str = cell.InnerText;
                                str = WebUtility.HtmlDecode(str);// remove("&nbsp;", "");
                                str.Replace(System.Environment.NewLine, "");
                                str.Replace("\r", String.Empty).Replace("\n", String.Empty);
                                String[] sArr = Regex.Split(str, @"(?:\r\n|\n|\r)");
                                for (int i = 0; i < sArr.Length; ++i)
                                {
                                    sArr[i] = sArr[i].Trim();
                                    sArr[i].Replace(" ", String.Empty);
                                }
                                String newStr = String.Join(String.Empty, sArr);
                                newStr.Trim();
                                // to get for example 25/02/2018
                                Match mat = Regex.Match(newStr, @"(?:[012]?[0-9]|3[01])[./-](?:0?[1-9]|1[0-2])[./-](?:[0-9]{2}){1,2}");//[./-](?:[0-9]{2}){1,2} this works for /02   /2018
                                while (mat.Success)
                                {
                                    String sValue = mat.Value;
                                    bool bRet = DateTime.TryParseExact(sValue, "dd/MM/yyyy", System.Globalization.CultureInfo.InvariantCulture, DateTimeStyles.None, out rrTable.m_iDate);
                                    Debug.Assert(bRet);
                                    // todo to compare with the date in the link.
                                    mat = mat.NextMatch();
                                }
                                String[] strs = newStr.Split(' ');
                                //Console.WriteLine(strs);
                            }
                        }
                    } // end of for each row of a table. 
                }
            } // end of for all tables.
        } 

        public static void Main()
        {
            const bool bChineseVersion = false; // false: english; true: chinese. 

            //testWebRequest();
            if (false)
            {
                // to debug one page 
                // case: the date is tomorrow, or the race today do not finish.
                String sDebugLink =
                    //"http://racing.hkjc.com/racing/info/meeting/Results/chinese/Local/20180225/ST/11"
                    "http://racing.hkjc.com/racing/info/meeting/Results/chinese/Local/20180303/ST/8"
                    ;
                SortedList<Int32, RacingDate> aDebugRacingDate = new SortedList<int, RacingDate>();
                GetRacingDates(sDebugLink, aDebugRacingDate);

                RRTable rrTable = new RRTable();
                GetResultTableFromLink(sDebugLink, rrTable);
                int a = 0;
                int b = a; ++b;
            }

            String sDateFolder = @"D:\horseRacingTemp";
            // this table should have some data do not need to update, open need to append new data:
            // date, local, field, page id, link, racing id, ...
            String sRacingDatePageIdLinkFile;
            if (bChineseVersion)
                sRacingDatePageIdLinkFile = Path.Combine(sDateFolder, "racing_Date_Local_Field_PageID_PageLink.csv");
            else
                sRacingDatePageIdLinkFile = Path.Combine(sDateFolder, "racing_Date_Local_Field_PageID_PageLink_english.csv");
            // how many racing date 

            // to load the (date, pageid, pagelink) from file, 
            SortedList<Int32, RacingDate> aRacingDateFromFile = new SortedList<int, RacingDate>();
            int iReadRowCountFromFile = RacingDate.readDateFromFile(aRacingDateFromFile, sRacingDatePageIdLinkFile);
            Console.WriteLine($@"Finish loading historical data: date, local, field, pageid, pagelink, from file {sRacingDatePageIdLinkFile},
                date count {sRacingDatePageIdLinkFile.Count()}, row count {iReadRowCountFromFile}");

            // key: 20180304, value: RacingDate 
            SortedList<Int32, RacingDate> aRacingDate = new SortedList<int, RacingDate>();
            bool bDebugOneDate = false;// true;//
            if (bDebugOneDate)
            {
                // for debug, only one date, 
                RacingDate rd = new RacingDate();
                rd.m_dateTime = new DateTime(2017, 06, 04);
                rd.m_localType = "Local";
                rd.m_field = "ST";
                aRacingDate.Add(20170604, rd);
            }
            else
            {
                // to get the latest whole racing dates, we can do this by using one page. 
                String sTempLink;
                if (bChineseVersion)
                    sTempLink = "http://racing.hkjc.com/racing/info/meeting/Results/chinese/Local/20180225/ST/11";
                else
                    sTempLink = "http://racing.hkjc.com/racing/info/meeting/Results/English/Local/20180225/ST/11";
                GetRacingDates(sTempLink, aRacingDate);
                // some dates have racing local, while other dates have racing overside, 
            }
            Console.WriteLine($"Finish collecing latest date, date count {aRacingDate.Count} ...");
            // After collecting the racing date, 
            // the next step is to find out how many racing at each date. 
            Int32 iTry10 = 0; 
            while (iTry10++ < 10) 
            {
                bool bSomeDateNeedUpdated = false; 
                foreach (KeyValuePair<Int32, RacingDate> kvp in aRacingDate)
                {
                    Int32 iDate = kvp.Key;
                    RacingDate rd = kvp.Value;

                    bool bToPassThisDate = false;
                    bool bToPassFindingPageIdLinkForTheDateAlreadyInFile = true;
                    if (bToPassFindingPageIdLinkForTheDateAlreadyInFile)
                    {
                        if (aRacingDateFromFile.ContainsKey(iDate))
                        {
                            // file already has this day's record.
                            // 某一天的所有page ids, page links, 都是可以从某一天的一个page中就能找到的, 
                            // 所以要么就只找到默认的那个pageid/link 这个时候就只有1个, 要么就都能找到, 
                            if (aRacingDateFromFile[iDate].m_aPageIdToLink.Count > 3)
                            {
                                // assumption that we have found all page ids and links for this day, no need to do it again
                                bToPassThisDate = true;
                            }
                        }
                    }
                    if (bToPassThisDate)
                    {
                        Console.WriteLine($"Pass   collecing page ids and links of date {iDate}, already count {aRacingDateFromFile[iDate].m_aPageIdToLink.Count}.");
                        continue;
                    }
                    if (rd.m_dateTime.Date > DateTime.Today.Date)
                    {
                        // 例如今天是2018/3/12, 上面收集到页面上有2018/3/14, 我们也跳过14号的不收集它的pageid pagelink, 因为很可能还是3/11的.
                        Console.WriteLine($@"Pass collecing page ids and links of date {iDate}, it is ahead of today.");
                        continue;
                    }

                    // to build a link to start the search the first page 1. 
                    String sCurrentLink = String.Empty;
                    if (rd.m_localType.IndexOf("Local", StringComparison.OrdinalIgnoreCase) >= 0)
                    {
                        String sDate = rd.m_dateTime.ToString("yyyyMMdd");
                        String sTemp;
                        if (bChineseVersion)
                            sTemp = "http://racing.hkjc.com/racing/info/meeting/Results/chinese/Local/" + sDate + "/" + rd.m_field + "/1";
                        else
                            sTemp = "http://racing.hkjc.com/racing/info/meeting/Results/English/Local/" + sDate + "/" + rd.m_field + "/1"; ; 

                        sCurrentLink = sTemp;
                    }
                    if (sCurrentLink == String.Empty)
                    {
                        continue;
                    }
                    bSomeDateNeedUpdated = true;
                    // how many pages/racing in that current day. 
                    GetPageIdAndLink(sCurrentLink, rd.m_aPageIdToLink);
                    //foreach (var link in aPageIdToLink)
                    //    Console.WriteLine("    " + link.Key + ": " + link.Value);
                    Console.WriteLine($"Finish collecing page ids and links of date {iDate}, count {rd.m_aPageIdToLink.Count}.");

                    // compare to the date recorded in file
                    if (aRacingDateFromFile.ContainsKey(iDate) == false)
                    {
                        aRacingDateFromFile.Add(iDate, rd);
                    }
                    else
                    {
                        // this date is already record in the file, however, no sure if the data of page id is complete.
                        // to add the missing page id and page link. 
                        foreach (KeyValuePair<Int32, String> kvpPageId2Link in rd.m_aPageIdToLink)
                        {
                            int iPageIdFound = kvpPageId2Link.Key;
                            String sPageLinkFound = kvpPageId2Link.Value;
                            if (aRacingDateFromFile[iDate].m_aPageIdToLink.ContainsKey(iPageIdFound) == false)
                            {
                                aRacingDateFromFile[iDate].m_aPageIdToLink.Add(iPageIdFound, sPageLinkFound);
                            }
                            else
                            {
                                // the link for this page id should be the same.
                                Debug.Assert(aRacingDateFromFile[iDate].m_aPageIdToLink[iPageIdFound] == sPageLinkFound);
                            }
                        }
                    }
                } // end of for each date.
                List<Int32> aToRemoveDateAhead = new List<Int32>();

                int iRowCountWriteToFile = RacingDate.writeDateToFile(aRacingDateFromFile, sRacingDatePageIdLinkFile);
                Console.WriteLine($@"Finish update historical data: date, local, field, pageid, pagelink, to file {sRacingDatePageIdLinkFile},
                date count {aRacingDateFromFile.Count}, totally row count {iRowCountWriteToFile}");
                //
                // if all dates (to collect pageid and pageLink) can be passed this round, no date need to update, then break;
                if (bSomeDateNeedUpdated == false)
                {
                    break;
                }
            } // end of while a few times.
              // Notice, if no date is updated at the first round, means all date have all the pageids pageLinks,
              // means the file is already updated.
              // aRacingDate is temp for collecting date, it would have no links 
              // aRacingDateFromFile is updated. 

            // After geting all dates, all pageids pageLinks for each date,
            // then the next step is to map (date+pageid <-> race id), 
            String sFileOfRaceIdMapDatePageId = Path.Combine(sDateFolder, "racing_raceid_raceid_date_pageid.csv");
            if (bChineseVersion == false)
            {
                sFileOfRaceIdMapDatePageId = Path.Combine(sDateFolder, "racing_raceid_raceid_date_pageid_english.csv");
            }
            // key: iYearUid; value: RaceIdMapDatePageId.
            SortedList<Int64, RaceIdMapDatePageId> aRaceIdMapDatePageId = new SortedList<Int64, RaceIdMapDatePageId>();
            RaceIdMapDatePageId.readRowsFromFile(aRaceIdMapDatePageId, sFileOfRaceIdMapDatePageId);
            Console.WriteLine($@"Finish loading race id, count {aRaceIdMapDatePageId.Count}, from file {sFileOfRaceIdMapDatePageId}.");

            // to build an inverted mapping 
            // key: date, pageid, iYearUid
            SortedList<Int32, SortedList<Int32, Int64>> aDatePageIdMapRaceId = new SortedList<int, SortedList<int, Int64>>();
            foreach (KeyValuePair<Int64, RaceIdMapDatePageId> kvp in aRaceIdMapDatePageId)
            {
                Int64 iYearUid = kvp.Key;
                Int32 iDate = RacingDate.DateTimeToInt( kvp.Value.m_aDate);
                Int32 iPageId = kvp.Value.m_iPageId;
                 
                if (aDatePageIdMapRaceId.ContainsKey(iDate) == false)
                    aDatePageIdMapRaceId.Add(iDate, new SortedList<int, Int64>());
                aDatePageIdMapRaceId[iDate].Add(iPageId, iYearUid);
            }

            // then the next step is to collect racing result for each link...
            String sFileOfRaceResult = "racing_historical_results.csv";
            if (!bChineseVersion)
                sFileOfRaceResult = "racing_historical_results_english.csv";
            String sAppDir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().GetName().CodeBase);
            //sAppDir.Remove("file:\\", 0);
            sFileOfRaceResult = Path.Combine(sDateFolder, sFileOfRaceResult);

            SortedList<Int64, RRTable> aRaceIdToResult = new SortedList<Int64, RRTable>();
            RRTable.ReadRaceResultFromFile(aRaceIdToResult, sFileOfRaceResult);
            Console.WriteLine($"Finish loading historical results, raceid count {aRaceIdToResult.Count}, from file.");

            int iTry100 = 0;
            while (iTry100++ < 100)
            {
                bool bThisRoundHasSthUpdate = false;
                //int axx = 0; 
                foreach (KeyValuePair<Int32, RacingDate> kvp in aRacingDateFromFile)
                {
                    //if (++axx > 20)
                    //    break;
                    Int32 iDate = kvp.Key;
                    RacingDate rd = kvp.Value; 
                    SortedList<Int32, String> aPageIdToLink = rd.m_aPageIdToLink; 

                    foreach (KeyValuePair<Int32, String> kvp2 in aPageIdToLink)
                    {
                        int iPageId = kvp2.Key;

                        bool bPassUpdateResult = false;
                        // from date+pageid to raceid.
                        if (aDatePageIdMapRaceId.ContainsKey(iDate))
                        {
                            if (aDatePageIdMapRaceId[iDate].ContainsKey(iPageId))
                            {
                                Int64 iYearUid = aDatePageIdMapRaceId[iDate][iPageId];
                                if (aRaceIdToResult.ContainsKey(iYearUid))
                                    bPassUpdateResult = true;
                            }
                        } 

                        if (bPassUpdateResult)
                        { 
                        }
                        else
                        { 
                            String sLink = kvp2.Value;
                            RRTable rrTable = new RRTable();
                            GetResultTableFromLink(sLink, rrTable);
                            if (rrTable.m_iDate.Year > 1900 == false)
                            {
                                bool bRet = DateTime.TryParseExact(iDate.ToString(), "yyyyMMdd", System.Globalization.CultureInfo.InvariantCulture, DateTimeStyles.None, out rrTable.m_iDate);
                                Debug.Assert(bRet);
                            }

                            if (rrTable.m_iUid != -1)
                            {
                                // to get raceid from date+pageid
                                Int64 iYearUid = RaceId.YearRaceId(rrTable.m_iDate, rrTable.m_iUid);
                                aRaceIdToResult[iYearUid] = rrTable;
                                Console.WriteLine($"Finish collecting result from {iDate}, raceId {iYearUid}.");

                                if (aRaceIdMapDatePageId.ContainsKey(iYearUid) == false)
                                {
                                    RaceIdMapDatePageId raceIdMapDatePageId = new RaceIdMapDatePageId();
                                    raceIdMapDatePageId.m_raceId.m_iYearUid = iYearUid;
                                    raceIdMapDatePageId.m_aDate = rrTable.m_iDate;
                                    raceIdMapDatePageId.m_iPageId = iPageId;

                                    aRaceIdMapDatePageId.Add(iYearUid, raceIdMapDatePageId);
                                    int iDateTemp = RacingDate.DateTimeToInt(rrTable.m_iDate);
                                    if (aDatePageIdMapRaceId.ContainsKey(iDateTemp) == false)
                                        aDatePageIdMapRaceId.Add(iDateTemp, new SortedList<int, Int64>());
                                    aDatePageIdMapRaceId[iDateTemp].Add(iPageId, iYearUid);
                                } 
                            }
                            bThisRoundHasSthUpdate = true;
                        }
                    }  
                     
                    //Thread.Sleep(1000);//1 second.
                } // end of for each date.
                if (bThisRoundHasSthUpdate == false)
                    break;

                RaceIdMapDatePageId.writeRowsToFile(aRaceIdMapDatePageId, sFileOfRaceIdMapDatePageId);
                Console.WriteLine("Begin to write race results to file " + sFileOfRaceResult);
                Int32 iRaceIdWroteCount = RRTable.WriteRaceResultToCSVFile(aRaceIdToResult, sFileOfRaceResult);
                Console.WriteLine($@"End of writing race results to file {sFileOfRaceResult}, raceid count {iRaceIdWroteCount}.");
            } // end of  while (iTry100++ < 100)

            //
            Console.WriteLine("Please input a key to end ...");
            Console.ReadKey();
        } // end of func Main()
    }
}
 
