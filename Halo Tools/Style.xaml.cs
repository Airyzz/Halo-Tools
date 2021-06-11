using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace HaloTools
{
    partial class Style
    {
        public Style()
        {
            InitializeComponent();
        }

        private void StatusBar_MouseDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            ((Window)((FrameworkElement)sender).TemplatedParent).DragMove();
        }

        private void Thumb_ManipulationDelta(object sender, System.Windows.Input.ManipulationDeltaEventArgs e)
        {

        }
    }
}
