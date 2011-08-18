using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.ComponentModel;
using iTextSharp.text;
using iTextSharp.text.pdf;
using System.IO;


namespace ReportToPDF
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
                table.AddCell(i);
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
                using (Document doc = new Document())
                {
                    // step 2:
                    // we create a writer that listens to the document
                    // and directs a PDF-stream to a file

                    PdfWriter writer = PdfWriter.GetInstance(doc,
                                       new FileStream("1.pdf", FileMode.Create));

                    // step 3: we open the document
                    doc.Open();

                    doc.Add(new Paragraph("Simple Image"));

                    //Jpeg jpg = new Jpeg(new System.Uri("file:///C:/_ISC/2011wjb/ReportToPDF/bin/Debug/1.jpeg"));
                    Image jpg = Image.GetInstance(System.Environment.CurrentDirectory+"/../../xp1.bmp");
                    doc.Add(jpg);

                    Paragraph p = new Paragraph("US Presidents Born in xxx");
                    p.Alignment = 1;
                    doc.Add(p);
                    // table data, see code snippet following this one
                    doc.Add(_stateTable());

                    //doc.Close(); // Dispose() do Close()
                }


                Console.WriteLine("--end--, PDF has been created");
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }

            // step 5: we close the document
        }
    }
}
