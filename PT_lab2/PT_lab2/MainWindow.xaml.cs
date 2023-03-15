using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Forms;
using System.Windows.Markup;
using System.IO;
using System.Diagnostics;
using Microsoft.VisualBasic.FileIO;

namespace PT_lab2
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 



    public partial class MainWindow : Window
    {
        private static ContextMenu fileMenu, directoryMenu;
        static void create_file_tree(DirectoryInfo current_dir, ref TreeViewItem current_menu_root)
        {
            current_menu_root.ContextMenu = fileMenu;

            current_menu_root.Header = current_dir.Name;
            current_menu_root.Tag = current_dir.FullName;
            FileInfo[] files = current_dir.GetFiles();

            // writing files
            for (int i = 0; i < files.Length; i++)
            {
                var newFileItem = new TreeViewItem
                {
                    Header = files[i].Name,
                    Tag = files[i].FullName
                };

                newFileItem.ContextMenu = fileMenu;

                current_menu_root.Items.Add(newFileItem);
            }

            DirectoryInfo[] dirs = current_dir.GetDirectories();
            for (int i = 0; i < dirs.Length; i++)
            {

                var new_menu_root = new TreeViewItem();
                current_menu_root.Items.Add(new_menu_root);

                create_file_tree(dirs[i], ref new_menu_root);
            }
        }

        private void new_tree_file_selected(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            TreeViewItem selectedItem = e.NewValue as TreeViewItem;

            if (selectedItem != null)
            {
                string filePath = selectedItem.Tag.ToString();
                StatusText.Text = get_rash(new FileInfo(filePath));
            }
            else
            {
                StatusText.Text = "";
            }
        }

        static string get_rash(FileSystemInfo file_sys_info)
        {
            string read_only = "-", archive = "-", system = "-", hidden = "-";

            if (file_sys_info.Attributes.HasFlag(FileAttributes.ReadOnly))
            {
                read_only = "r";
            }

            if (file_sys_info.Attributes.HasFlag(FileAttributes.Archive))
            {
                archive = "a";
            }

            if (file_sys_info.Attributes.HasFlag(FileAttributes.System))
            {
                system = "s";
            }

            if (file_sys_info.Attributes.HasFlag(FileAttributes.Hidden))
            {
                hidden = "h";
            }

            return read_only + archive + system + hidden;
        }
        private void openFile(object sender, RoutedEventArgs e)
        {
            ContextMenu contextMenu = (sender as MenuItem).Parent as ContextMenu;

            Trace.Write(contextMenu.ToString());

            TreeViewItem treeViewItem = contextMenu.PlacementTarget as TreeViewItem;

            string filePath = treeViewItem.Tag.ToString();

            string text = File.ReadAllText(filePath);

            TextArea.Text = text;

            //Process.Start("cmd", $"/c start \"\" \"{filePath}\"");
        }

        private void browseFiles(object sender, RoutedEventArgs e)
        {
            if (FileTree.Items.Count == 1)
            {
                FileTree.Items.RemoveAt(0); //removing root
            }

            string rootName = null, rootPath = null;

            var folderBrowser = new FolderBrowserDialog() { Description = "Select directory to open" };
            if (folderBrowser.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                rootName = new DirectoryInfo(folderBrowser.SelectedPath).Name;
                rootPath = folderBrowser.SelectedPath;
            }

            var root = new TreeViewItem
            {
                Header = rootName,
                Tag = rootPath
            };

            create_file_tree(new DirectoryInfo(rootPath), ref root);

            FileTree.Items.Add(root);
        }

        void exitFromProgram(object sender, RoutedEventArgs e)
        {
            System.Windows.Application.Current.Shutdown();
        }

        private void deleteAtPath(object sender, RoutedEventArgs e)
        {
            ContextMenu contextMenu = (sender as MenuItem).Parent as ContextMenu;

            Trace.Write(contextMenu.ToString());

            TreeViewItem treeViewItem = contextMenu.PlacementTarget as TreeViewItem;

            Trace.Write(treeViewItem.Header);

            TreeViewItem rootItem = treeViewItem.Parent as TreeViewItem;

            rootItem.Items.Remove(treeViewItem);

            string filePath = treeViewItem.Tag.ToString();

            if (File.Exists(filePath))
            {
                FileSystem.DeleteFile(filePath, UIOption.OnlyErrorDialogs, RecycleOption.SendToRecycleBin);
            }
            else if (Directory.Exists(filePath))
            {
                FileSystem.DeleteDirectory(filePath, UIOption.OnlyErrorDialogs, RecycleOption.SendToRecycleBin);
            }
        }

        public MainWindow()
        {
            InitializeComponent();

            fileMenu = new ContextMenu();
            MenuItem openFileOption = new MenuItem { Header = "Open" };
            MenuItem deleteFileOption = new MenuItem { Header = "Delete" };
            openFileOption.Click += openFile;
            deleteFileOption.Click += deleteAtPath;
            fileMenu.Items.Add(openFileOption);
            fileMenu.Items.Add(deleteFileOption);

            directoryMenu = new ContextMenu();
            MenuItem deleteFolderOption = new MenuItem { Header = "Delete" };
            deleteFolderOption.Click += deleteAtPath;
            directoryMenu.Items.Add(deleteFolderOption);
        }
    }
}
