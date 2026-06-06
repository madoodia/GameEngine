param(
	[string]$WorkspaceRoot = $PSScriptRoot
)

Add-Type -AssemblyName System.Windows.Forms

# Create form
$form = New-Object System.Windows.Forms.Form
$form.Text = "Create C++ Module"
$form.Width = 350
$form.Height = 180
$form.StartPosition = "CenterScreen"
$form.TopMost = $true

# Directory label
$labelDir = New-Object System.Windows.Forms.Label
$labelDir.Text = "Module Path:"
$labelDir.Left = 20
$labelDir.Top = 40
$labelDir.Width = 65
$labelDir.Height = 20
$form.Controls.Add($labelDir)

# Directory textbox with placeholder
$textBoxDir = New-Object System.Windows.Forms.TextBox
$textBoxDir.Text = "Engine\Source\Core\"
$textBoxDir.ForeColor = [System.Drawing.Color]::Black
$textBoxDir.Left = 90
$textBoxDir.Top = 38
$textBoxDir.Width = 200
$textBoxDir.Height = 25
$form.Controls.Add($textBoxDir)

# OK button
$okButton = New-Object System.Windows.Forms.Button
$okButton.Text = "OK"
$okButton.Left = 100
$okButton.Top = 80
$okButton.Width = 100
$okButton.Height = 30
$okButton.DialogResult = [System.Windows.Forms.DialogResult]::OK
$form.Controls.Add($okButton)
$form.AcceptButton = $okButton

# Cancel button
$cancelButton = New-Object System.Windows.Forms.Button
$cancelButton.Text = "Cancel"
$cancelButton.Left = 205
$cancelButton.Top = 80
$cancelButton.Width = 100
$cancelButton.Height = 30
$cancelButton.DialogResult = [System.Windows.Forms.DialogResult]::Cancel
$form.Controls.Add($cancelButton)
$form.CancelButton = $cancelButton

$result = $form.ShowDialog()

if ($result -ne [System.Windows.Forms.DialogResult]::OK) {
	exit 0
}

# Get output directory (remove placeholder if still there)
$OutputDir = $textBoxDir.Text.Trim()
$ModuleName = $OutputDir.Split('\')[-1]
$OutputDir = $OutputDir.TrimEnd('\')
$FullPath = Join-Path $WorkspaceRoot $OutputDir

Write-Host "Output Directory: $OutputDir" -ForegroundColor Cyan
Write-Host "Module Name: $ModuleName" -ForegroundColor Magenta
Write-Host "Full Path: $FullPath" -ForegroundColor Yellow

$ParentDir = Split-Path $FullPath -Parent
if (-not (Test-Path -Path $ParentDir)) {
	New-Item -ItemType Directory -Path $ParentDir -Force | Out-Null
}

if($ModuleName -eq ""){
	exit 1
}

if ($ModuleName -ne $OutputDir){
# Create cpp file
$cppFile = "$FullPath.cpp"
$cppContent = @"
// (C) 2026 madoodia.com
// ---------------------

#include "$ModuleName.h"
"@
Set-Content -Path $cppFile -Value $cppContent -Encoding UTF8
}
else{
	$FullPath = Join-Path $WorkspaceRoot "Engine\Source\$ModuleName"
}

# create header file
$headerfile = "$Fullpath.h"
$headercontent = @"
// (c) 2026 madoodia.com
// ---------------------

#pragma once

#include "defines.h"

"@

set-content -path $headerfile -value $headercontent -encoding utf8

# show success message
$message = "`nFile(s) created successfully!`n`n---> Header: $headerfile`n---> Source: $cppfile"

# print the message in the terminal
write-host $message -ForegroundColor Green
