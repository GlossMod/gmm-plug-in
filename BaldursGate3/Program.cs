using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LSLib.LS;

namespace BaldursGate3
{
    public class Program
    {
        // static void Main(string[] args)
        // {
        //     LoadModDataFromPakAsync(@"C:\Users\xiaom\AppData\Local\Larian Studios\Baldur's Gate 3\Mods\ShortRestTweaks.pak").await((result) =>
        //     {
        //         System.Console.WriteLine(result);
        //     });
        //     // System.Console.WriteLine(text.await());
        // }

        // 从 pak 中加载 mod 数据
        // LSLib By: https://github.com/Norbyte/lslib
        public static async Task<object> LoadModDataFromPakAsync(string pakPath)
        {
            using (var pr = new LSLib.LS.PackageReader(pakPath))
            {
                var pak = pr.Read();
                var metaFiles = pak.Files.Where(f => f.Name.EndsWith("meta.lsx")).ToList();

                foreach (var item in metaFiles)
                {
                    using (var stream = item.MakeStream())
                    {
                        using (var sr = new System.IO.StreamReader(stream))
                        {
                            string text = await sr.ReadToEndAsync();

                            return text;
                        }
                    }
                }
            }

            return null;
        }
    }
}
