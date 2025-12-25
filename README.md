# Employee Attendance Management System - Qt GUI Application

## Overview
This is a complete Qt-based GUI application for managing employee attendance, leave requests, and feedback. The application is converted from a console-based system to provide a modern, user-friendly interface.

## Features

### Admin Interface
- **Employee Management**: Add, edit, delete employees with complete CRUD operations
- **Leave Request Management**: Approve or reject employee leave requests
- **Feedback Management**: View all employee feedback submissions
- **Tabbed Interface**: Organized sections for different administrative tasks

### Employee Interface
- **Dashboard**: Personal information display and attendance status
- **Attendance Marking**: One-click attendance marking with date/time tracking
- **Leave Requests**: Submit leave requests with date range and reason
- **Leave Status**: View status of submitted leave requests (Pending/Approved/Rejected)
- **Feedback Submission**: Submit feedback to management

## System Requirements
- Qt 6.x or later
- C++17 compatible compiler
- Windows (tested), Linux, or macOS

## Installation and Setup

### Prerequisites
1. Install Qt Creator and Qt framework (6.x recommended)
2. Ensure Qt development tools are properly configured

### Building the Application
1. Open Qt Creator
2. Open the project file: `AttendanceManagement.pro`
3. Configure the project for your kit (compiler/Qt version)
4. Build the project (Ctrl+B or Build menu)
5. Run the application (Ctrl+R or Run menu)

### Command Line Build (Alternative)
```bash
# Navigate to project directory
cd "c:\Users\AT\Desktop\EAS Project"

# Generate Makefiles
qmake AttendanceManagement.pro

# Build the application
nmake  # On Windows with MSVC
# or
make   # On Linux/macOS or Windows with MinGW
```

## Usage

### First Time Setup
1. Launch the application
2. Data files will be created automatically in your Documents/EAS_Data folder
3. Default credentials:
   - **Admin**: Username: `admin`, Password: `admin123`
   - **Employee**: ID: `1001`, Password: `1001` (default test employee)

### Admin Login
- Select "Admin Login" from the welcome screen
- Use admin credentials to access administrative features
- Manage employees, approve leave requests, and view feedback

### Employee Login
- Select "Employee Login" from the welcome screen
- Enter employee ID and password
- Access personal dashboard, mark attendance, submit leave requests

## File Structure
```
AttendanceManagement/
├── main.cpp                    # Application entry point
├── mainwindow.cpp/.h          # Main window and navigation
├── loginDialog.cpp/.h         # Login authentication
├── adminInterface.cpp/.h      # Admin panel implementation
├── employeeInterface.cpp/.h   # Employee dashboard
├── employee.cpp/.h            # Employee data model
├── leaveRequest.cpp/.h        # Leave request data model
├── attendanceRecord.cpp/.h    # Attendance data model
├── *.ui                       # Qt Designer form files
├── resources.qrc              # Resource file (icons, etc.)
└── AttendanceManagement.pro   # Qt project file
```

## Data Storage
The application uses text-based file storage:
- `employees.txt` - Employee information
- `leave_requests.txt` - Leave request records
- `attendance.txt` - Attendance records
- `feedback.txt` - Employee feedback

Files are automatically created in: `Documents/EAS_Data/`

## Default Test Data
- **Admin Login**: admin/admin123
- **Test Employee**: ID 1001, Password: 1001
  - Name: John Doe
  - Department: IT
  - Position: Developer
  - Salary: $50,000

## Key Features Implemented

### Modern Qt GUI
- Professional styling with CSS-like stylesheets
- Responsive layout with splitters and proper sizing
- Tabbed interface for organized functionality
- Modern form controls (date pickers, spin boxes, etc.)

### Data Management
- File-based persistence using QFile and QTextStream
- Proper error handling and validation
- Real-time data updates across interfaces

### User Experience
- Role-based access control
- Input validation and user feedback
- Status messages and confirmations
- Intuitive navigation and workflow

### Security
- Password-based authentication
- Role separation (Admin vs Employee)
- Data validation and sanitization

## Extending the Application

### Adding New Features
1. **Database Integration**: Replace file-based storage with SQLite or other databases
2. **Reports**: Add reporting functionality for attendance and leave statistics
3. **Notifications**: Implement email/SMS notifications for leave approvals
4. **Multiple Roles**: Add more user roles (HR, Manager, etc.)

### Customization
- Modify stylesheets in each interface for different themes
- Add company branding by replacing icons and colors
- Extend data models for additional employee information

## Troubleshooting

### Common Issues
1. **Build Errors**: Ensure Qt development environment is properly configured
2. **File Access**: Check write permissions for Documents folder
3. **Missing Dependencies**: Verify Qt modules are installed (widgets, core)

### Debug Mode
- Run in Qt Creator debug mode for detailed error information
- Check console output for file operation messages
- Verify data file creation in Documents/EAS_Data

## License
This is a sample application for educational and demonstration purposes.

## Support
For technical support or questions about the Qt implementation:
- Review Qt documentation: https://doc.qt.io/
- Check Qt forums: https://forum.qt.io/
- Consult Qt examples and tutorials
