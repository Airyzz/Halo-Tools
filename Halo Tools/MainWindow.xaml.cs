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
using Airyz;
using static HaloTools.Addresses;

namespace HaloTools
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        public AiryzMemory halo;

        public MainWindow()
        {
            InitializeComponent();

            halo = new AiryzMemory("MCC-Win64-Shipping");
            ReadAllAddresses(halo);
        }

        private void FOVSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            halo?.WriteFloat(fov, (float)FOVSlider.Value);
        }
    }
}
