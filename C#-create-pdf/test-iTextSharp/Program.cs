using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.ComponentModel;
using iTextSharp.text;
using iTextSharp.text.pdf;
using System.IO;

//  Don't forget to set the correct Content-Type before you send the PDF stream if you're running a web application:
//Response.ContentType = "application/pdf";
//// [optional]
//Response.AddHeader(
//  "Content-Disposition",
//  "attachment; filename=itext.pdf"
//);

//Just repeat 
//cb.beginText(); 
//cb.setFontAndSize(bf, size); 
//... 
//cb.endText(); 
//for every page. 否则可能其他内容换页，文本不换页

// FontFactory.GetFont("Arial", 10, Font.BOLD);
// ColumnText.ShowTextAligned(cb, PdfContentByte.ALIGN_LEFT,  new Phrase(dateString, fontForDate), 100, 100, 0);
// 用 contentByte.ShowTextAligned 无法指定bold等

namespace test_iTextSharp
{
    class Program
    {
        private static PdfPTable _stateTable()
        {
            string[] col = { "No.", "Name", "City" };
            PdfPTable table = new PdfPTable(3);
            /*
            * default table width => 80%
            */
            table.WidthPercentage = 100;
            // then set the column's __relative__ widths
            table.SetWidths(new Single[] { 1, 5, 4 });
            /*
            * by default tables 'collapse' on surrounding elements,
            * so you need to explicitly add spacing
            */
            table.SpacingBefore = 10;

            for (int i = 0; i < col.Length; ++i)
            {
                PdfPCell cell = new PdfPCell(new Phrase(col[i]));
                cell.BackgroundColor = new BaseColor(204, 204, 204);
                table.AddCell(cell);
            }

            // !! database code omitted !!
            // r.Read is the DbDataReader for whatever flavor 
            // of database you're connecting to; we're iterating
            // over the results returned from the database and 
            // adding rows to the table in the PDF 
            foreach (var i in new string[] { "hello", "world" })
            {
                table.AddCell(i.Length.ToString());
                table.AddCell(i + i);
                table.AddCell(i + i + i);
            }
            return table;
        }
        static void Main(string[] args)
        {
            try
            {
                Console.WriteLine("--begin--");

                // step 1: creation of a document-object
                Rectangle pageSize = new Rectangle(PageSize.A4.Rotate()); // new Rectangle(144, 720);
                 // A4=29.7cm*21cm=842*595
                pageSize.BackgroundColor = new BaseColor(0xFF, 0xFF, 0xDE); 
                //using (Document doc = new Document())
                using (Document doc = new Document(pageSize, 10, 10, 10, 10)) // in pt,磅; 72pt=1 inch=2.54 centimeters=1/12feet
                {  // default margin 36pt
                    // step 2:
                    // we create a writer that listens to the document
                    // and directs a PDF-stream to a file

                    PdfWriter writer = PdfWriter.GetInstance(doc,
                                       new FileStream("1.pdf", FileMode.Create));
                    // 可以创建多个 writer，可以 pause

                    //after got writer
                    doc.AddTitle("example"); // in pdf's property
                    doc.AddSubject("subject-1");
                    // step 3: we open the document
                    doc.Open();

                    // 左下角为原点
                    PdfContentByte cb = writer.DirectContent;
                    cb.SetLineWidth(3f);
                    //cb.SetColorFill(BaseColor.GREEN);
                    cb.SetColorStroke(BaseColor.RED);
                    cb.MoveTo(10, 70);
                    cb.LineTo(100, -180);
                    cb.Circle(10, 100, 50);
                    cb.Stroke();

                    BaseFont bf = BaseFont.CreateFont(BaseFont.HELVETICA, 
                        BaseFont.CP1252, BaseFont.NOT_EMBEDDED);
                    cb.BeginText(); 
                    cb.SetFontAndSize(bf, 12); 
                    cb.ShowTextAligned(PdfContentByte.ALIGN_CENTER, "This text is centered", 50, 70, 0); 
                    cb.EndText();

                    

                    ColumnText ct = new ColumnText(cb);
                    ct.SetSimpleColumn(60, 30, 100, 150, 15, Element.ALIGN_CENTER);
                    ct.AddText(new Phrase("phrase1"));
                    ct.AddText(new Phrase("phrase2"));
                    ct.AddText(new Phrase("phrase3"));
                    ct.Go(); 

                    doc.Add(new Paragraph("Simple Image"));

                    //Jpeg jpg = new Jpeg(new System.Uri(""));
                    //Image img = Image.GetInstance("..\\..\\xp1.bmp"); // fail
                    //Image img = Image.GetInstance(System.Environment.CurrentDirectory+"/../../xp1.bmp");
                    Image img = Image.GetInstance(new FileStream("..\\..\\xp1.bmp", FileMode.Open)); // ok
                    doc.Add(img);

                    //cb.SaveState();
                    cb.BeginText();
                    img.SetAbsolutePosition(30, 180);
                    img.ScaleAbsolute(200, 180);
                    cb.AddImage(img);
                    cb.EndText();
                    //cb.RestoreState();

                    Paragraph p = new Paragraph("US Presidents Born in xxx");
                    p.Alignment = 1;
                    //p.SetAbsolutePosition(100, 0);
                    doc.Add(p);

                    // table data, see code snippet following this one
                    doc.Add(_stateTable());

                    doc.NewPage(); // 换页

                    Chunk chunk2 = new Chunk("This font is of type ITALIC | STRIKETHRU",
                        FontFactory.GetFont(FontFactory.HELVETICA, 12, Font.ITALIC | Font.STRIKETHRU));
                    chunk2.SetBackground(new BaseColor(0xFF, 0xFF, 0x00)); 
                    doc.Add(chunk2);
                    chunk2.SetTextRise(2);
                    doc.Add(chunk2);

                    doc.Add(new Paragraph(100));

                    var myPhrase = new Phrase("Hello 1bis! ", FontFactory.GetFont(FontFactory.TIMES_ROMAN, 8, Font.BOLD));
                    myPhrase.Add(new Phrase("some other font ", FontFactory.GetFont(FontFactory.HELVETICA, 8, Font.ITALIC)));
                    myPhrase.Add(new Phrase("This is the end of the sentence.\n", FontFactory.GetFont(FontFactory.TIMES_ROMAN, 8, Font.ITALIC)));
                    doc.Add(myPhrase); 
                    //doc.Close(); // Dispose() do Close()  //关闭所有 writer

                    doc.Add(new Paragraph(100));

                    // Chunk -> Phrase -> Paragraph
                    Anchor anchor = new Anchor("website:http://itextsharp.sourceforge.net", 
                        FontFactory.GetFont(FontFactory.HELVETICA, 12, Font.UNDERLINE, new BaseColor(0, 0, 255)));
                    anchor.Reference = "http://itextsharp.sourceforge.net";
                    anchor.Name = "website";
                    doc.Add(anchor);

                    doc.Add(new Paragraph(100));

                    Anchor anchor1 = new Anchor("This is an internal link",
                        FontFactory.GetFont(FontFactory.HELVETICA, 12, Font.UNDERLINE, new BaseColor(0, 0, 255)));
                    anchor1.Name = "link1";
                    anchor1.Reference = "#website";
                    doc.Add(anchor1);

                    List list = new List(true, 20); 
                    list.Add(new ListItem("First line")); 
                    list.Add(new ListItem("The second line is longer to see what happens once the end of the line "+
                        "is reached. Will it start on a new line?"
                        + "The second line is longer to see what happens once the end of the line " +
                        "is reached. Will it start on a new line?")); 
                    list.Add(new ListItem("Third line")); 
                    doc.Add(list);

                    List overview = new List(false, 10);
                    overview.ListSymbol = new Chunk("\u2022", FontFactory.GetFont(FontFactory.HELVETICA, 20));
                    overview.Add(new ListItem("This is an item"));
                    overview.Add("This is another item");
                    doc.Add(overview);

                    Annotation a = new Annotation("authors", "Maybe it's because I wanted to be an author myself that I wrote iText");
                    doc.Add(a);

                    Annotation annot = new Annotation(100f, 700f, 200f, 800f, new Uri("http://www.lowagie.com"));
                    doc.Add(annot);
                    Annotation annot2 = new Annotation(100f, 700f, 200f, 800f, "http://www.lowagie.com", "LOWAGIE");
                    doc.Add(annot2);
                    // ??

                    //Graphic grx = new Graphic();
                    ////添加一个矩形 
                    //grx.rectangle(100, 700, 100, 100);
                    //// 添加一条斜线 
                    //grx.moveTo(100, 700);
                    //grx.lineTo(200, 800);
                    //// 将图形显示出来 
                    //grx.stroke();
                    //doc.Add(grx); 
                    // deprecated ?

 
                    BaseFont bfHei = BaseFont.CreateFont(System.Environment.ExpandEnvironmentVariables(@"%windir%\Fonts\SIMHEI.TTF"),
                        BaseFont.IDENTITY_H, BaseFont.NOT_EMBEDDED);
                    Font font = new Font(bfHei, 32);
                    String text = "这是黑体字测试！";
                    doc.Add(new Paragraph(text, font));

                    
                }
                Console.WriteLine("--end--, PDF has been created");
                //System.Diagnostics.Process.Start("1.pdf", ""); // not work when foxit already open
                //System.Diagnostics.Process.Start("cmd.exe", " /c start 1.pdf");
                System.Diagnostics.Process.Start("ACroRd32.exe", "1.pdf");


            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }

            // step 5: we close the document
        }
    }
}
