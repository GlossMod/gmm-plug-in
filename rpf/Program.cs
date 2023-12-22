using System;
using RageLib.GTA5.Cryptography;

namespace Rpf
{

    class Program
    {

        static void Main(string[] args)
        {
            // 获取当前程序所在目录
            string path = System.AppDomain.CurrentDomain.BaseDirectory;
            GTA5Constants.LoadFromPath(path);

            /**
             * cmd 使用 
             * rpf read "rpfFile" "file"
             * rpf write "rpfFile" "file" "inputPath"
             * rpf create "InputFolder" "output" "name" 
             */
            if (args.Length < 3)
            {
                Console.WriteLine("参数不足。");
                return;
            }

            string command = args[0];
            string rpf = args[1];
            string filename = args[2];

            switch (command)
            {
                case "read":
                    Read(rpf, filename);
                    break;
                case "write":
                    if (args.Length < 4)
                    {
                        Console.WriteLine("写入操作需要一个输入文件参数.");
                        return;
                    }
                    string inputFile = args[3];
                    Write(rpf, filename, inputFile);
                    break;
                case "create":
                    if (args.Length < 4)
                    {
                        Console.WriteLine("创建操作需要一个输出文件参数.");
                        return;
                    }
                    string output = args[2];
                    string name = args[3];
                    Create(rpf, output, name);
                    break;
                default:
                    Console.WriteLine("未知的命令.");
                    break;
            }
        }

        static void Read(string rpf, string filename)
        {
            var data = RpfHandler.ReadData(rpf, filename);
            // 转换为字符串并打印
            // string text = System.Text.Encoding.Default.GetString(data);
            System.Console.WriteLine(data);
        }

        static void Write(string rpf, string filename, string inputFile)
        {
            RpfHandler.InputFile(rpf, filename, inputFile);
        }

        static void Create(string inputFolder, string output, string name)
        {
            RpfHandler.CreateArchive(inputFolder, output, name);
        }
    }
}
