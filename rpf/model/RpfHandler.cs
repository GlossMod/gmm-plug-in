using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using NUnit.Framework.Constraints;
using RageLib.Archives;
using RageLib.Data;
using RageLib.GTA5.Archives;
using RageLib.GTA5.ArchiveWrappers;
using RageLib.GTA5.Cryptography;
using RageLib.GTA5.Resources.PC;

class RpfHandler
{
    /// <summary>
    /// 读取文件
    /// </summary>
    /// <param name="rpfFile">rpf文件路径</param>
    /// <param name="file">rpf文件内部文件路径</param>
    public static byte[] ReadData(string rpfFile, string file)
    {
        var rpf = RageArchiveWrapper7.Open(rpfFile);

        // 从 file 中获取目录
        string directoryName = Path.GetDirectoryName(file);
        string fileName = Path.GetFileName(file);

        byte[] buf = null;

        IArchiveFile AFile;
        if (directoryName == "")
        {
            AFile = rpf.Root.GetFile(fileName);
        }
        else
        {
            System.Console.WriteLine(directoryName);
            IArchiveDirectory directory = GetListDirectory(rpf.Root, directoryName);
            AFile = directory.GetFile(fileName);
        }

        if (AFile != null)
        {
            if (AFile is IArchiveBinaryFile binFile)
            {
                var ms = new MemoryStream();
                binFile.Export(ms);
                buf = new byte[ms.Length];
                ms.Position = 0;
                ms.Read(buf, 0, buf.Length);

                if (binFile.IsEncrypted)
                {
                    var qq = GTA5Hash.CalculateHash(binFile.Name);
                    var gg = (qq + (uint)binFile.UncompressedSize + (101 - 40)) % 0x65;
                    // TODO: if archive encrypted with AES, use AES key...
                    buf = GTA5Crypto.Decrypt(buf, GTA5Constants.PC_NG_KEYS[gg]);
                }
                if (binFile.IsCompressed)
                {
                    var def = new DeflateStream(new MemoryStream(buf), CompressionMode.Decompress);
                    var bufnew = new byte[binFile.UncompressedSize];
                    def.Read(bufnew, 0, (int)binFile.UncompressedSize);
                    buf = bufnew;
                }
            }
            else
            {
                var ms = new MemoryStream();
                AFile.Export(ms);
                ms.Position = 0;

                buf = new byte[ms.Length];
                ms.Position = 0;
                ms.Read(buf, 0, buf.Length);
            }
        }
        else
        {
            System.Console.WriteLine("文件不存在");
        }

        rpf.Dispose();
        return buf;
    }

    /// <summary>
    /// 写入文件
    /// </summary>
    /// <param name="rpfFile">rpf文件路径</param>
    /// <param name="file">需要导入的文件</param>
    /// <param name="inputPath">导入文件在rpf中的路径</param>
    public static void InputFile(string rpfFile, string file, string inputPath)
    {
        // 1. 打开rpf 文件
        RageArchiveWrapper7 rpf = RageArchiveWrapper7.Open(rpfFile);
        System.Console.WriteLine(rpf.FileName);

        // 获取导入文件的目录和名称
        var folder = Path.GetDirectoryName(inputPath);
        string name = Path.GetFileName(inputPath);

        IArchiveDirectory directory;

        // 获取或创建文件夹
        if (folder == "")
        {
            directory = rpf.Root;
        }
        else
        {
            directory = GetListDirectory(rpf.Root, folder);
        }

        System.Console.WriteLine($"directory.Name:{directory.Name}");

        var oldFile = directory.GetFile(name);

        if (oldFile != null)
        {
            directory.DeleteFile(oldFile);
        }


        // 开始写入文件
        bool isResource = false;
        foreach (var type in ResourceFileTypes_GTA5_pc.AllTypes)
        {
            if (file.EndsWith(type.Extension))
            {
                if (type == ResourceFileTypes_GTA5_pc.Meta)
                {
                    using (var ms = new FileStream(file, FileMode.Open))
                    {
                        var reader = new DataReader(ms, Endianess.BigEndian);
                        var ident = reader.ReadUInt32();
                        ms.Position = 0;
                        if (ident == 0x5053494E)
                        {
                            var binFile = directory.CreateBinaryFile();
                            binFile.Name = name;
                            binFile.Import(ms);
                        }
                        else
                        {
                            isResource = true;
                            var resource = directory.CreateResourceFile();
                            resource.Name = name;
                            resource.Import(ms);
                        }
                    }
                }
                else
                {
                    isResource = true;
                    var resource = directory.CreateResourceFile();
                    resource.Name = name;
                    resource.Import(file);
                }
                break;
            }
        }
        if (!isResource)
        {
            var binFile = directory.CreateBinaryFile();
            binFile.Name = name;
            binFile.Import(file);
        }
        rpf.Flush();
        rpf.Dispose();
    }


    /// <summary>
    /// 通过文件夹创建 rpf
    /// </summary>
    /// <param name="InputFolder"></param>
    /// <param name="output"></param>
    /// <param name="name"></param>
    public static void CreateArchive(string InputFolder, string output, string name)
    {
        // 这个方法来自 https://github.com/gizzdev/gtautil/blob/master/gtautil/Program/CreateArchive.cs
        // this function by https://github.com/gizzdev/gtautil/blob/master/gtautil/Program/CreateArchive.cs

        string rpfPath = $@"{output}\{name}rpf";
        using (RageArchiveWrapper7 rpf = RageArchiveWrapper7.Create(rpfPath))
        {
            var queue = new List<Tuple<string, IArchiveDirectory, RageArchiveWrapper7>>() {
                        new Tuple<string, IArchiveDirectory, RageArchiveWrapper7>(InputFolder, rpf.Root, rpf)
                    };

            var subRpfs = new List<Tuple<DirectoryInfo, IArchiveDirectory, RageArchiveWrapper7>>();

            rpf.archive_.Encryption = RageArchiveEncryption7.NG;

            var rpfs = new List<RageArchiveWrapper7>();

            while (queue.Count > 0)
            {
                var folder = queue[0].Item1;
                var curr = queue[0].Item2;
                var currRpf = queue[0].Item3;

                if (rpfs.IndexOf(currRpf) == -1)
                    rpfs.Add(currRpf);

                Console.WriteLine(folder);

                queue.RemoveAt(0);

                string newFolder = null;
                IArchiveDirectory newCurr = null;
                RageArchiveWrapper7 newCurrRpf = null;

                string[] folders = Directory.GetDirectories(folder);
                string[] files = Directory.GetFiles(folder);

                for (int i = 0; i < folders.Length; i++)
                {
                    var folderInfo = new DirectoryInfo(folders[i]);

                    newFolder = folders[i];

                    if (folders[i].EndsWith(".rpf"))
                    {
                        var tmpStream = new FileStream(Path.GetTempFileName(), FileMode.Open);
                        var subRpf = RageArchiveWrapper7.Create(tmpStream, folderInfo.Name);

                        subRpf.archive_.Encryption = RageArchiveEncryption7.NG;

                        subRpfs.Add(new Tuple<DirectoryInfo, IArchiveDirectory, RageArchiveWrapper7>(folderInfo, curr, subRpf));

                        newCurr = subRpf.Root;
                        newCurrRpf = subRpf;
                    }
                    else
                    {
                        var directory = curr.CreateDirectory();
                        directory.Name = folderInfo.Name;

                        newCurr = directory;
                        newCurrRpf = currRpf;
                    }

                    queue.Add(new Tuple<string, IArchiveDirectory, RageArchiveWrapper7>(newFolder, newCurr, newCurrRpf));

                }

                if (folders.Length + files.Length == 0)
                {
                    Console.WriteLine("  .\\.empty");
                    var binFile = curr.CreateBinaryFile();
                    binFile.Name = ".empty";

                    var ms = new MemoryStream(1);

                    ms.WriteByte(0);

                    ms.Flush();

                    binFile.Import(ms);
                }

                for (int i = 0; i < files.Length; i++)
                {
                    string file = files[i];
                    var fileInfo = new FileInfo(file);

                    bool isResource = false;

                    for (int j = 0; j < ResourceFileTypes_GTA5_pc.AllTypes.Count; j++)
                    {
                        var type = ResourceFileTypes_GTA5_pc.AllTypes[j];

                        if (file.EndsWith(type.Extension))
                        {
                            Console.WriteLine("  " + file);

                            if (type == ResourceFileTypes_GTA5_pc.Meta)
                            {
                                using (var ms = new FileStream(file, FileMode.Open))
                                {
                                    var reader = new DataReader(ms, Endianess.BigEndian);
                                    var ident = reader.ReadUInt32();
                                    ms.Position = 0;
                                    if (ident == 0x5053494E)
                                    {
                                        var binFile = curr.CreateBinaryFile();
                                        binFile.Name = fileInfo.Name;
                                        binFile.Import(ms);
                                    }
                                    else
                                    {
                                        isResource = true;

                                        var resource = curr.CreateResourceFile();
                                        resource.Name = fileInfo.Name;
                                        resource.Import(ms);
                                    }
                                }
                            }
                            else
                            {
                                isResource = true;

                                var resource = curr.CreateResourceFile();
                                resource.Name = fileInfo.Name;
                                resource.Import(file);
                            }

                            break;
                        }
                    }

                    if (!isResource)
                    {
                        Console.WriteLine("  " + file);
                        var binFile = curr.CreateBinaryFile();
                        binFile.Name = fileInfo.Name;
                        binFile.Import(file);
                    }
                }

            }

            rpfs.Reverse();

            for (int i = 0; i < subRpfs.Count; i++)
            {
                var subRpf = subRpfs[i];

                var file = subRpf.Item2.CreateBinaryFile();
                file.Name = subRpf.Item1.Name;

                subRpf.Item3.Flush();

                file.Import(subRpf.Item3.archive_.BaseStream);
            }

            for (int i = 0; i < rpfs.Count; i++)
            {
                rpfs[i].Flush();

                if (i + 1 < rpfs.Count)
                {
                    var stream = (FileStream)rpfs[i].archive_.BaseStream;
                    string fileName = stream.Name;

                    rpfs[i].Dispose();

                    File.Delete(fileName);
                }
            }
        }
    }


    /// <summary>
    /// 获取最终的文件夹
    /// </summary>
    /// <param name="Root"></param>
    /// <param name="directoryName"></param>
    /// <returns></returns>
    static IArchiveDirectory GetListDirectory(IArchiveDirectory Root, string directoryName)
    {
        IArchiveDirectory directory;
        System.Console.WriteLine(directoryName);
        //  分割路径 为 数组
        string[] directorys = directoryName.Split(Path.DirectorySeparatorChar);

        if (directorys.Length > 1)
        {
            directory = Root.GetDirectory(directorys[0]);

            if (directory == null)
            {
                directory = Root.CreateDirectory();
                directory.Name = directorys[0];
            }

            for (int i = 1; i < directorys.Length; i++)
            {
                directory = directory.GetDirectory(directorys[i]);
                if (directory == null)
                {
                    directory = Root.CreateDirectory();
                    directory.Name = directorys[i];
                }
            }
        }
        else
        {
            directory = Root.GetDirectory(directoryName);
            if (directory == null)
            {
                directory = Root.CreateDirectory();
                directory.Name = directoryName;
            }
        }

        return directory;
    }

}