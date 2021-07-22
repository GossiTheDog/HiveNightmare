# Fix HiveNightmare ACLs and snapshots
# v1.0

# Originally by unknown and adapted by @doctormay6 and @GossiTheDog

# Schedule to run as SYSTEM in a deployment tool, test locally first
# Do not run on Windows Server in case you use VSS for backups

#change permissions and delete shadows
$checkPermissions = icacls c:\Windows\System32\config\sam
if ($checkPermissions -like '*BUILTIN\Users:(I)(RX*)*') {
    icacls c:\windows\system32\config\*.* /inheritance:e
    vssadmin delete shadows /quiet /all
    $vulnerable = $true
}
else {
    $vulnerable = $false
}

 

#check permissions
if ($vulnerable -eq $true) {
    $checkPermissions = icacls C:\windows\system32\config\sam
    if ($checkPermissions -like '*BUILTIN\Users:(I)(RX*)*') {
        $permissionsSucces = $false
        write-host "ACL change failed. Check permissions running script, e.g. run as SYSTEM."
    }
    else {
        $permissionsSucces = $true
        Write-Host "Successfully reset permission inheritance on affected files."
    }
}

 

#check shadow
if ($vulnerable -eq $true) {
    $checkShadow = Get-WmiObject Win32_ShadowStorage -Property UsedSpace | Select-Object -ExpandProperty UsedSpace
    if (0 -eq $checkShadow) {
        $shadowSucces = $true
        Write-Host "Successfully deleted old volume shadow copies."
    }
    else {
        $shadowSucces = $false
        write-host "Shadow deletion failed. Security software may be blocking this action or check running permissions."
    }
}

 

#check if fixed logic
if ($vulnerable -eq $true) {
    if ($permissionsSucces -eq $true -and $shadowSucces -eq $true) {
        $fixed = $true
    }
    else {
        $fixed = $false
    }
}
else {
    $fixed = 'Not applicable'
}

 

#create new shadow
if ($vulnerable -eq $true -and $shadowSucces -eq $true -and $permissionsSucces -eq $true) {
    wmic shadowcopy call create Volume='C:\'
    Write-Host ""
}

 

#output data
write-host "vulnerable: $vulnerable"
write-host "Fixed: $fixed"
