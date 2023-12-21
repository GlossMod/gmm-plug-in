

// using System.Collections.Generic;
// using System.Security.AccessControl;
// using RageLib.Archives;

// class Directory
// {
//     public Directory parentDirectory;
//     public IArchiveDirectory directory;
//     private List<Directory> childDirectories;

//     public ICollection<Directory> Children
//     {
//         get
//         {
//             return childDirectories;
//         }
//     }

//     public string Name
//     {
//         get
//         {
//             if (parentDirectory == null)
//                 return "";
//             else
//                 return directory.Name;
//         }
//     }

//     public Directory(IArchiveDirectory directory, Directory parent)
//     {

//         this.directory = directory;
//         this.parentDirectory = parent;
//         // this.files = directory.GetFiles();

//         childDirectories = new List<Directory>();
//         foreach (var f in directory.GetDirectories())
//         {
//             var dd = new Directory(f, this);
//             childDirectories.Add(dd);
//         }
//     }
// }