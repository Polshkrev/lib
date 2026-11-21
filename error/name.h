#ifndef NAMES_H_
#define NAMES_H_

/**
 * @brief Indentity of an error.
 */
typedef enum
{
    ErrorOk, // No error has occured. This inidicates a success branch.
    ErrorBase, // Base error name.
    AllocationError, // For any allocation error.
    ArithmeticError, // For any miscellaneous arithmatic exception.
	OverflowError, // For any overflow.
	UnderflowError, // For any underflow.
	ZeroDivisionError, // When dividing by zero.
	AssertionError, // For any failed assertion.
	EOFError, // When reaching the end of a file or buffer.
	LookupError, // When any miscellaneous lookup fails.
	OutOfRangeError, // For any indexed access outside of the allotted range.
	IndexError, // For any indexed access.
	KeyError, // For any keyed access.
	OSError, // For any miscellaneous operating system exception.
	IOError, // For any miscellaneous io exceptions.
	BlockingIOError, // For any miscellaneous blocking io exception.
	ChildProcessError, // For any miscellaneous child process exception.
	ConnectionError, // For any miscellaneous connection exception.
	BrokenPipeError, // For any exception relating to a broken pipe.
	ConnectionAbortedError, // For an aborted connection.
	ConnectionRefusedError, // For a refused connection.
	ConnectionResetError, // For a reset connection.
	FileExistsError, // When a file exists already in the file system.
	FileNotFoundError, // When a file cannot be found.
	IsADirectoryError, // When an accessed file is a directory.
	NotADirectoryError, // When an accessed directory is not a directory.
	PermissionError, // When an accessed file or endpoint does not have the correct permissions
	ProcessLookupError, // For a porcess lookup fail.
	TimeoutError, // For a timeout occurrence.
	RuntimeError, // For any miscellaneous runtime exception.
	NotImplementedError, // For any unimplemented methods or functions.
	ValueError, // For any miscellaneous value exception.
	UnreachableError, // For an unreachable case.
} ErrorName;

const char *error_name_to_string(ErrorName name);

#endif // NAMES_H_

#ifdef ERROR_NAME_IMPLEMENTATION

const char *error_name_to_string(ErrorName name)
{
    switch (name)
    {
        case ErrorOk:
        {
            return "Ok";
        } break;
        case ErrorBase:
        {
            return "ErrorBase";
        } break;
        case AllocationError:
        {
            return "AllocationError";
        } break;
        case ArithmeticError:
        {
            return "ArithmeticError";
        } break;
        case OverflowError:
        {
            return "OverflowError";
        } break;
        case UnderflowError:
        {
            return "UnderflowError";
        } break;
        case ZeroDivisionError:
        {
            return "ZeroDivisionError";
        } break;
        case AssertionError:
        {
            return "AssertionError";
        } break;
        case EOFError:
        {
            return "EOFError";
        } break;
        case LookupError:
        {
            return "LookupError";
        } break;
        case OutOfRangeError:
        {
            return "OutOfRangeError";
        } break;
        case IndexError:
        {
            return "IndexError";
        } break;
        case KeyError:
        {
            return "KeyError";
        } break;
        case OSError:
        {
            return "OSError";
        } break;
        case IOError:
        {
            return "IOError";
        } break;
        case BlockingIOError:
        {
            return "BlockingIOError";
        } break;
        case ChildProcessError:
        {
            return "ChildProcessError";
        } break;
        case ConnectionError:
        {
            return "ConnectionError";
        } break;
        case BrokenPipeError:
        {
            return "BrokenPipeError";
        } break;
        case ConnectionAbortedError:
        {
            return "ConnectionAbortedError";
        } break;
        case ConnectionRefusedError:
        {
            return "ConnectionRefusedError";
        } break;
        case ConnectionResetError:
        {
            return "ConnectionResetError";
        } break;
        case FileExistsError:
        {
            return "FileExistsError";
        } break;
        case FileNotFoundError:
        {
            return "FileNotFoundError";
        } break;
        case IsADirectoryError:
        {
            return "IsADirectoryError";
        } break;
        case NotADirectoryError:
        {
            return "NotADirectoryError";
        } break;
        case PermissionError:
        {
            return "PermissionError";
        } break;
        case ProcessLookupError:
        {
            return "ProcessLookupError";
        } break;
        case TimeoutError:
        {
            return "TimeoutError";
        } break;
        case RuntimeError:
        {
            return "RuntimeError";
        } break;
        case NotImplementedError:
        {
            return "NotImplementedError";
        } break;
        case ValueError:
        {
            return "ValueError";
        } break;
        case UnreachableError:
        {
            return "UnreachableError";
        } break;
        default:
        {
            return "Ok";
        } break;
    }
}

#endif // ERROR_NAME_IMPLEMENTATION