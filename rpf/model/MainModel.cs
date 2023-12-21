/*
    Copyright(c) 2015 Neodymium

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
using RageLib.Archives;
using RageLib.Data;
using RageLib.GTA5.ArchiveWrappers;
using RageLib.GTA5.Cryptography;
using System.IO;
using System.IO.Compression;

public class MainModel
{
    private string fileName;
    private IArchive archive;


    public IArchive Archive
    {
        get
        {
            return archive;
        }
    }

    public bool HasKeys
    {
        get
        {
            if (GTA5Constants.PC_AES_KEY != null)
                return true;
            else
                return false;
        }
    }

    public MainModel()
    {
        // 获取当前程序所在目录
        string path = System.AppDomain.CurrentDomain.BaseDirectory;
        GTA5Constants.LoadFromPath(path);
    }




    /// <summary>
    /// 创建 Creates an archive.
    /// </summary>
    public void New(string fileName)
    {
        // close first...
        Close();

        this.archive = RageArchiveWrapper7.Create(fileName);
        this.fileName = fileName;
    }

    /// <summary>
    /// 打开 Opens an archive.
    /// </summary>
    public void Load(string fileName)
    {
        // close first...
        Close();

        this.archive = RageArchiveWrapper7.Open(fileName);
        this.fileName = fileName;
    }

    /// <summary>
    /// 关闭 Closes an archive.
    /// </summary>
    public void Close()
    {
        if (archive != null)
            archive.Dispose();
        archive = null;
        fileName = null;
    }






    /// <summary>
    /// Imports a file.
    /// </summary>
    public void Import(IArchiveDirectory directory, string fileName)
    {


        var fi = new FileInfo(fileName);

        var fs = new FileStream(fileName, FileMode.Open);
        var fsR = new DataReader(fs);
        var ident = fsR.ReadUInt32();
        fs.Close();


        // delete existing file
        var existingFile = directory.GetFile(fi.Name);
        if (existingFile != null)
            directory.DeleteFile(existingFile);


        if (ident == 0x07435352)
        {

            var newF = directory.CreateResourceFile();
            newF.Name = fi.Name;
            newF.Import(fileName);

        }
        else
        {

            var newF = directory.CreateBinaryFile();
            newF.Name = fi.Name;
            newF.Import(fileName);

        }

    }

    /// <summary>
    /// 导出 Exports a file.
    /// </summary>
    public void Export(IArchiveFile file, string fileName)
    {

        if (file is IArchiveBinaryFile)
        {
            var binFile = (IArchiveBinaryFile)file;

            // export
            var ms = new MemoryStream();
            file.Export(ms);
            ms.Position = 0;

            var buf = new byte[ms.Length];
            ms.Position = 0;
            ms.Read(buf, 0, buf.Length);

            // decrypt...
            if (binFile.IsEncrypted)
            {
                var qq = GTA5Hash.CalculateHash(binFile.Name);
                var gg = (qq + (uint)binFile.UncompressedSize + (101 - 40)) % 0x65;

                // TODO: if archive encrypted with AES, use AES key...

                buf = GTA5Crypto.Decrypt(buf, GTA5Constants.PC_NG_KEYS[gg]);
            }

            // decompress...
            if (binFile.IsCompressed)
            {
                var def = new DeflateStream(new MemoryStream(buf), CompressionMode.Decompress);
                var bufnew = new byte[binFile.UncompressedSize];
                def.Read(bufnew, 0, (int)binFile.UncompressedSize);
                buf = bufnew;
            }

            File.WriteAllBytes(fileName, buf);
        }
        else
        {

            file.Export(fileName);

        }

    }

    /// <summary>
    /// 删除 Deletes a file.
    /// </summary>
    public void Delete()
    { }



    public void FindConstants(string exeFileName)
    {
        var exeData = File.ReadAllBytes(exeFileName);

        GTA5Constants.Generate(exeData);
        GTA5Constants.SaveToPath(".");
    }
}