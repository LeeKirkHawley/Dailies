#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <algorithm>
#include <SqliteCpp/SQLiteCpp.h>
#include "DailiesDatabase.h"

#include "Day.h"


using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Dailies::implementation
{
    DailiesDatabase m_dbManager;
    SQLite::Database m_database = m_dbManager.OpenAppDatabase("C:\\temp\\Dailies.db");
    

    void MainWindow::Initialize()
    {
        m_dbManager.Migrate(m_database);
    }

    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void MainWindow::Splitter_PointerPressed(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
    {
        auto splitter = sender.as<winrt::Microsoft::UI::Xaml::UIElement>();
        m_dragging = splitter.CapturePointer(e.Pointer());
        m_dragVertical = (sender == VerticalSplitter());
        auto point = e.GetCurrentPoint(nullptr).Position();
        if (m_dragVertical)
        {
            m_start = point.X;
            m_startSize = LeftCol().ActualWidth();
        }
        else
        {
            m_start = point.Y;
            m_startSize = TopRow().ActualHeight();
        }
        e.Handled(true);
    }

    void MainWindow::Splitter_PointerMoved(
        winrt::Windows::Foundation::IInspectable const&,
        winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
    {
        if (!m_dragging) return;
        auto point = e.GetCurrentPoint(nullptr).Position();
        if (m_dragVertical)
        {
            auto width = (std::max)(80.0, m_startSize + (point.X - m_start));
            LeftCol().Width(winrt::Microsoft::UI::Xaml::GridLengthHelper::FromPixels(width));
        }
        else
        {
            auto height = (std::max)(80.0, m_startSize + (point.Y - m_start));
            TopRow().Height(winrt::Microsoft::UI::Xaml::GridLengthHelper::FromPixels(height));
        }
        e.Handled(true);
    }

    void MainWindow::Splitter_PointerReleased(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
    {
        if (!m_dragging) return;
        sender.as<winrt::Microsoft::UI::Xaml::UIElement>().ReleasePointerCapture(e.Pointer());
        m_dragging = false;
        e.Handled(true);
    }

    void MainWindow::AddRecurringTaskButton_Click(
        winrt::Windows::Foundation::IInspectable const& /* sender */,
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const& /* e */)
    {
        Day newDay;
        newDay.date = "2024-06-11"; // Example date, replace with actual value
        m_dbManager.insertDay(m_database, newDay);

        Day day = m_dbManager.findDayByDateString(m_database, "2024-06-11");  

        Task newTask;
        newTask.dayId = day.id;
        newTask.taskName = "New Recurring Task";
        newTask.isDone = "false";
        newTask.createdUtc = "2024-06-11T12:00:00Z"; // Example timestamp, replace with actual value
        newTask.type = "Recurring";


        m_dbManager.insertTask(m_database, day, newTask);
    }
}
    