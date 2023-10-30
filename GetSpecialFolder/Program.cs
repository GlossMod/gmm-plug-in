using System;
using System.Threading.Tasks;

namespace GetSpecialFolder
{
    public class Program
    {
        // 获取我的文档所在目录
        public async Task<object> GetMyDocuments(object param)
        {
            return Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
        }


        // 获取其他路径
        public async Task<object> GetOther(object param)
        {
            return Environment.GetFolderPath((System.Environment.SpecialFolder)param);
        }
    }
}
