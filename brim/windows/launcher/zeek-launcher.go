// +build windows

// This tool executes zeek on windows, constructing the cygwin compatible ZEEK*
// environment variables required.  It embeds knowledge of the locations of the
// zeek executable and zeek script locations in the expanded 'zdeps/zeek'
// directory inside a Brim installation.
//
// It uses the Windows "job object" api:
// https://docs.microsoft.com/en-us/windows/win32/procthread/job-objects
// to ensure that the launched zeek process is terminated when this process
// exits.
package main

import (
	"log"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
	"unsafe"

	"github.com/alexbrainman/ps"
	"github.com/alexbrainman/ps/winapi"
)

// These paths are relative to the zdeps/zeek directory.
var (
	zeekExecRelPath  = "bin/zeek.exe"
	zeekPathRelPaths = []string{
		"share/zeek",
		"share/zeek/policy",
		"share/zeek/site",
	}
	zeekPluginRelPaths = []string{
		"lib/zeek/plugins",
	}
)

func cygPathEnvVar(name, topDir string, subdirs []string) string {
	var s []string
	for _, l := range subdirs {
		p := filepath.Join(topDir, filepath.FromSlash(l))
		vol := filepath.VolumeName(p)
		cyg := "/cygdrive/" + vol[0:1] + filepath.ToSlash(p[len(vol):])
		s = append(s, cyg)
	}
	val := strings.Join(s, ":")
	return name + "=" + val
}

func launchZeek(zdepsZeekDir, zeekExecPath string, args []string) error {
	zeekPath := cygPathEnvVar("ZEEKPATH", zdepsZeekDir, zeekPathRelPaths)
	zeekPlugin := cygPathEnvVar("ZEEK_PLUGIN_PATH", zdepsZeekDir, zeekPluginRelPaths)

	cmd := exec.Command(zeekExecPath, args...)
	cmd.Stdin = os.Stdin
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	cmd.Env = append(os.Environ(), zeekPath, zeekPlugin)

	return cmd.Run()
}

// ensureSpawnedProcessTermination ensures that when this Go process terminates, the
// launched Zeek process will also be terminated. It does so via the Windows
// job objects api:
// https://docs.microsoft.com/en-us/windows/win32/procthread/job-objects
//
// See this Go issue for discussion about the challenge of process management
// on Windows, and the mention of the ps & winapi package used here:
// https://github.com/golang/go/issues/17608
func ensureSpawnedProcessTermination() error {
	// Create an unnamed job object; no name is necessary since no other
	// process needs to find or interact with it.
	jo, err := ps.CreateJobObject("")
	if err != nil {
		return err
	}

	// We add ourselves so that any process we launch will automatically be
	// added to the job.
	err = jo.AddCurrentProcess()
	if err != nil {
		return err
	}

	// We set the "kill on job close" option for the job, so that when the
	// last handle to the job is closed, all of the processes in the job
	// will be terminated.
	// This process is the only one with a handle to the job object, and we
	// intentionally leave it open. Like other handles, it will be closed
	// automatically when this process terminates. When that occurs, the
	// 'kill on job close' option will trigger the termination of any
	// spawned processes.
	limitInfo := winapi.JOBOBJECT_EXTENDED_LIMIT_INFORMATION{
		BasicLimitInformation: winapi.JOBOBJECT_BASIC_LIMIT_INFORMATION{
			LimitFlags: winapi.JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE,
		},
	}
	return winapi.SetInformationJobObject(jo.Handle, winapi.JobObjectExtendedLimitInformation,
		uintptr(unsafe.Pointer(&limitInfo)), uint32(unsafe.Sizeof(limitInfo)))
}

// zdepsZeekDirectory returns the absolute path of the zdeps/zeek directory,
// based on the assumption that this executable is located directly in it.
func zdepsZeekDirectory() (string, error) {
	execFile, err := os.Executable()
	if err != nil {
		return "", err
	}

	return filepath.Dir(execFile), nil
}

func main() {
	err := ensureSpawnedProcessTermination()
	if err != nil {
		log.Fatalln("ensureSpawnedProcessTermination failed:", err)
	}

	zdepsZeekDir, err := zdepsZeekDirectory()
	if err != nil {
		log.Fatalln("zdepsZeekDirectory failed:", err)
	}

	zeekExecPath := filepath.Join(zdepsZeekDir, filepath.FromSlash(zeekExecRelPath))
	if _, err := os.Stat(zeekExecPath); err != nil {
		log.Fatalln("zeek executable not found at", zeekExecPath)
	}

	err = launchZeek(zdepsZeekDir, zeekExecPath, os.Args[1:])
	if err != nil {
		log.Fatalln("launchZeek failed", err)
	}
}
