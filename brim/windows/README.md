# Summary

There's currently no formal support for Zeek on Windows, but the Brim application requires Zeek to create summary logs from PCAPs. We've therefore begun the effort of trying to port Zeek to Windows to support the minimum funcitonality required by Brim.

To assist with development, the scripts in this directory automate the creation of a Zeek artifact on a VM in Google Cloud. This includes:

* Creating the Windows Server instance
* Installing Cygwin and packages needed for building Zeek
* Configuring SSHD to allow for login without using Remote Desktop
* Installing Google Chrome for easier web access when using Remote Desktop
* Doing the build to assemble the artifact
* Creating a ZIP of the final artifact and copying it up to a storage bucket

# Limitations

While functional, the current approach is far from perfect.

* The Zeek artifact runs quite slow (maybe due to Cygwin).
* It only works with 32-bit Cygwin. While it builds successfully with 64-bit Cygwin, it doesn't produce output.

# Usage

## Running

```
./create-instance.sh
```

I've found a healthy build takes a bit over 30 minutes to complete.

## Shutdown

As indicated by the example output below, the VM is _not_ shutdown automatically, just in case you need to login and review any problems with the build process. Make sure you shut it down when you're done!

## Example output

```
Created [https://www.googleapis.com/compute/v1/projects/my-gcloud-project/zones/us-west1-a/instances/johndoe-windows-1].
NAME            ZONE        MACHINE_TYPE   PREEMPTIBLE  INTERNAL_IP  EXTERNAL_IP  STATUS
johndoe-windows-1  us-west1-a  n1-standard-8               10.138.x.x   35.247.x.x  RUNNING
Instance johndoe-windows-1 is booting. Sleeping for 10 seconds.
Instance johndoe-windows-1 is booting. Sleeping for 10 seconds.
...
Instance johndoe-windows-1 is configuring. Sleeping for 10 seconds.
...
Instance johndoe-windows-1 is rebooting. Sleeping for 10 seconds.
...
Zeek build is proceeding on johndoe-windows-1. Sleeping for 10 seconds.


Instance johndoe-windows-1 has finished booting. Login info:

ip_address: 35.247.x.x
password:   xxxxxxxxxxxxx
username:   johndoe

To set up for login without password prompt, paste password after each of:

ssh johndoe@35.247.x.x
ssh johndoe@35.247.x.x 'mkdir .ssh'
scp ~/.ssh/google_compute_engine.pub johndoe@35.247.x.x:.ssh/authorized_keys

After which you'll be able to:

gcloud compute ssh johndoe-windows-1
```

As the final output implies, you can perform a few manual steps that will make it easy for you to login via SSH without using the password every time. Of course you can feel free to access via Remote Desktop instead using the same password shown here. Or if you're satisfied with the artifact, just shut it down!

## Artifact

After a successful run like the one shown above, the finished artifact will have been uploaded to a storage bucket.

```
gs://my-bucket/windows-zeek-build/zeek-32.zip
```

## Debugging

If I'd wanted to check the serial port's output while it's running or review it for problems after the fact, I would have run:

```
gcloud compute instances get-serial-port-output johndoe-windows-1
```

I admit that the scripts are not yet super robust, so expect to be looking at the serial port output if you start making changes or the script seemed to finish suspiciously fast (<30 minutes). Even if the build failed halfway, you'll still get the SSH login info shown previously, so don't assume you have a solid artifact just because the script finished. Check its timestamp in the Google Cloud Storage UI!

## Configurable options

Configurable options are in `instance-setup-defaults.sh`. If you want a working artifact, you probably won't want to change any of these. Some notes on a couple key ones:

* `CYGWIN_ARCH` - If you set this to `64`, it will install the 64-bit Cygwin. As mentioned above, the 64-bit `zeek.exe` does run, but only produces a couple log file headers with no actual Zeek event data based on packets. We've not yet figured out why this happens.

* `CYGWIN_INSTALL_TYPE` - In its default setting of `local-install`, all packages are downloaded from a cache on a storage bucket at `gs://my-bucket/windows-zeek-build/cygwin`. We created this cache by doing a successful install with a set of packages downloaded from the Internet, then backed them up. This became necessary because at one point while putting together these scripts, a change had been made with the GCC version in Cygwin that prevented Zeek from building correctly. After learning this important lesson, we've now "locked in" a set of packages that we _know_ create a build with the code we've got in GitHub. If you want to have Cygwin instead come up with current downloaded copies of all the packages instead, set this variable to `download`.
