using System;
using System.Threading;
using Gtk;
using UI = Gtk.Builder.ObjectAttribute;

namespace BigBenTen
{
    class MainWindow : Window
    {
        //[UI] private Label _label1 = null;
        //[UI] private Button _button1 = null;

        [UI] private Label ClockLabel = null;
        [UI] private Label ClockLabel2 = null;
        [UI] private EventBox ClockEventBox = null;
        [UI] private EventBox ClockEventBox2 = null;
        [UI] private Notebook PageContainer = null;
        [UI] private Button AddAlarmButton = null;
        private Timer clockTimer = null;

        public MainWindow() : this(new Builder("MainWindow.glade")) {
            clockTimer = new Timer( UpdateClock , null, 0,1000);
        }

        private void UpdateClock(object state) {
            ClockLabel.Text = DateTime.Now.ToLongTimeString();
            ClockLabel2.Text = DateTime.Now.ToShortTimeString();
        }

        private MainWindow(Builder builder) : base(builder.GetRawOwnedObject("MainWindow"))
        {
            builder.Autoconnect(this);

            ClockEventBox.ButtonPressEvent += ClockPressed;
            ClockEventBox2.ButtonPressEvent += Clock2Pressed;

            DeleteEvent += Window_DeleteEvent;
        }

        private void ClockPressed(object o, ButtonPressEventArgs args)
        {
            PageContainer.Page = 1;
        }
        private void Clock2Pressed(object o, ButtonPressEventArgs args)
        {
            PageContainer.Page = 0;
        }
        private void AddAllarmButtonPresed(object o, ButtonPressEventArgs args)
        {
            PageContainer.Page = 0;
        }
        private void Window_DeleteEvent(object sender, DeleteEventArgs a)
        {
            Application.Quit();
        }

    }
}
