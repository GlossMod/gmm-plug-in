using System;
using Steamworks;
using System.Threading.Tasks;


namespace Steam
{
    public class Program
    {
        static void Main()
        {
            // 获取传递的参数 args
            string[] args = Environment.GetCommandLineArgs();

            new Program().GetWorkshopItems(args);
            // new Program().UGCDownload();
        }

        #region 下载文件
        public void UGCDownload()
        {
            if (SteamAPI.Init())
            {



                // string pchLocation = @"C:\Users\xiaom\Downloads\test\287\";
                // SteamAPICall_t apiCall = SteamRemoteStorage.UGCDownloadToLocation((UGCHandle_t)18446744073709551615, pchLocation, 0);
                // SteamAPICall_t apiCall = SteamRemoteStorage.UGCDownload((UGCHandle_t)18446744073709551615, 0);
                // SteamUGC.GetQueryUGCPreviewURL(UGCQueryHandle_t handle, uint index, out string pchURL, uint cchURLSize);

                // var callResult = CallResult<RemoteStorageDownloadUGCResult_t>.Create();
                // callResult.Set(apiCall, (RemoteStorageDownloadUGCResult_t param, bool bIOFailure) =>
                // {

                //     //打印 param.m_pchFileName 的所有参数 和值
                //     foreach (var item in param.GetType().GetProperties())
                //     {
                //         var name = item.Name;
                //         var value = item.GetValue(param, null);
                //         System.Console.WriteLine($"{name} | {value}");
                //     }
                // });

                // SteamScript.RunCallbacks(callResult);
            }
        }


        #endregion


        #region  获取物品列表
        public void GetWorkshopItems(string[] args)
        {
            if (SteamAPI.Init())
            {
                AppId_t appId = SteamUtils.GetAppID();  // Specifying your app id.
                UGCQueryHandle_t queryHandle = SteamUGC.CreateQueryAllUGCRequest(EUGCQuery.k_EUGCQuery_RankedByVote, EUGCMatchingUGCType.k_EUGCMatchingUGCType_Items, appId, appId, 1);

                SteamUGC.SetRankedByTrendDays(queryHandle, 90U);
                SteamUGC.SetAllowCachedResponse(queryHandle, 300U);
                SteamUGC.SetReturnChildren(queryHandle, true);
                SteamUGC.SetReturnMetadata(queryHandle, true);
                SteamUGC.SetReturnKeyValueTags(queryHandle, true);
                SteamUGC.SetReturnLongDescription(queryHandle, true);

                SteamScript.SendQueryUGCRequest<SteamUGCQueryCompleted_t>(queryHandle, OnWorkshopQueryCompleted);
            }
        }

        void OnWorkshopQueryCompleted(SteamUGCQueryCompleted_t pCallback, bool bIOFailure)
        {
            if (bIOFailure || pCallback.m_eResult != EResult.k_EResultOK)
            {
                return;
            }

            var numResults = pCallback.m_unNumResultsReturned;

            for (uint i = 0; i < numResults; i++)
            {
                if (SteamUGC.GetQueryUGCResult(pCallback.m_handle, i, out SteamUGCDetails_t details))
                {
                    if (details.m_eResult == EResult.k_EResultOK)
                    {
                        // Process the UGC details as needed
                        var itemId = details.m_nPublishedFileId;
                        var title = details.m_rgchTitle;
                        var fileSize = details.m_nFileSize;
                        // var FileId = details.m_nPublishedFileId;
                        var File = details.m_hFile;
                        var PreviewFile = details.m_hPreviewFile;   // 封面

                        SteamUGC.GetQueryUGCPreviewURL(pCallback.m_handle, i, out string url, 1024);

                        Console.WriteLine($"| {itemId} | {title} | {fileSize} |  {File} | {PreviewFile} | {url} ");
                    }
                }
            }

            // Always remember to release UGC query handles when you're done with them
            SteamUGC.ReleaseQueryUGCRequest(pCallback.m_handle);
        }

        #endregion

    }
}