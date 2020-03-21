#!/bin/bash
set -euo pipefail

source instance-setup-defaults.sh
SLEEP_TIME=10

gcloud compute instances create "${INSTANCE_NAME}" \
  --machine-type "${INSTANCE_TYPE}" \
  --boot-disk-size "${BOOT_DISK_SIZE}" \
  --image-project "$IMAGE_PROJECT" \
  --image-family "$IMAGE_FAMILY" \
  --metadata CYGWIN_ARCH="${CYGWIN_ARCH}",CYGWIN_INSTALL_TYPE="${CYGWIN_INSTALL_TYPE}" \
  --metadata-from-file windows-startup-script-ps1=startup-script.ps,BUILD_SCRIPT=build.sh \
  --scopes=storage-rw,https://www.googleapis.com/auth/logging.read

STATE="booting"
while [ "${STATE}" != "finished" ]; do
  if [ "${STATE}" = "booting" ]; then
    if LOGIN_INFO=$(gcloud -q compute reset-windows-password "${INSTANCE_NAME}" 2> /dev/null); then
      STATE="configuring"
    else
      echo "Instance ${INSTANCE_NAME} is booting. Sleeping for ${SLEEP_TIME} seconds."
      sleep ${SLEEP_TIME}
    fi
  elif [ "${STATE}" = "configuring" ]; then
    if gcloud compute instances get-serial-port-output "${INSTANCE_NAME}" 2> /dev/null | grep "Finished Configuring" > /dev/null ; then
      STATE="rebooting"
    else
      echo "Instance ${INSTANCE_NAME} is configuring. Sleeping for ${SLEEP_TIME} seconds."
      sleep ${SLEEP_TIME}
    fi
  elif [ "${STATE}" = "rebooting" ]; then
    if gcloud compute instances get-serial-port-output "${INSTANCE_NAME}" 2> /dev/null | grep "Proceeding with Zeek build" > /dev/null ; then
      STATE="building"
    else
      echo "Instance ${INSTANCE_NAME} is rebooting. Sleeping for ${SLEEP_TIME} seconds."
      sleep ${SLEEP_TIME}
    fi
  elif [ "${STATE}" = "building" ]; then
    if gcloud compute instances get-serial-port-output "${INSTANCE_NAME}" 2> /dev/null | grep "Finished running Zeek build script" > /dev/null ; then
      STATE="finished"
    else
      echo "Zeek build is proceeding on ${INSTANCE_NAME}. Sleeping for ${SLEEP_TIME} seconds."
      sleep ${SLEEP_TIME}
    fi
  fi
done

IP=$(echo "$LOGIN_INFO" | grep "ip_address:" | awk '{ print $2 }')
USERNAME=$(echo "$LOGIN_INFO" | grep "username:" | awk '{ print $2 }')
echo
echo "Instance ${INSTANCE_NAME} has finished booting. Login info:"
echo -e "\033[0;31m\033[1m"
echo "${LOGIN_INFO}"
echo -e "\033[0m"
echo "To set up for login without password prompt, paste password after each of:"
echo -e "\033[0;31m\033[1m"
echo "ssh ${USERNAME}@${IP}"
echo "ssh ${USERNAME}@${IP} 'mkdir .ssh'"
echo "scp ~/.ssh/google_compute_engine.pub ${USERNAME}@${IP}:.ssh/authorized_keys"
echo -e "\033[0m"
echo "After which you'll be able to:"
echo -e "\033[0;31m\033[1m"
echo "gcloud compute ssh ${INSTANCE_NAME}"
echo -e "\033[0m"
