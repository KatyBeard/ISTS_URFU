object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 416
  ClientWidth = 639
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 24
    Top = 21
    Width = 173
    Height = 13
    Caption = #1042#1099#1073#1077#1088#1080#1090#1077' '#1076#1080#1089#1082' '#1076#1083#1103' '#1089#1082#1072#1085#1080#1088#1086#1074#1072#1085#1080#1103
  end
  object Label2: TLabel
    Left = 24
    Top = 59
    Width = 118
    Height = 13
    Caption = #1042#1099#1073#1077#1088#1080#1090#1077' '#1073#1072#1079#1091' '#1076#1072#1085#1085#1099#1093
  end
  object ClusterSizeLabel: TLabel
    Left = 201
    Top = 165
    Width = 100
    Height = 13
    AutoSize = False
  end
  object Label7: TLabel
    Left = 24
    Top = 165
    Width = 116
    Height = 13
    Caption = #1056#1072#1079#1084#1077#1088' '#1082#1083#1072#1089#1090#1077#1088#1072', '#1041#1072#1081#1090
  end
  object Label6: TLabel
    Left = 24
    Top = 133
    Width = 116
    Height = 13
    Caption = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1082#1083#1072#1089#1090#1077#1088#1086#1074
  end
  object ClusterCountLabel: TLabel
    Left = 201
    Top = 133
    Width = 100
    Height = 13
    AutoSize = False
  end
  object FSSizeLabel: TLabel
    Left = 201
    Top = 101
    Width = 100
    Height = 13
    AutoSize = False
  end
  object Label3: TLabel
    Left = 24
    Top = 101
    Width = 152
    Height = 13
    Caption = #1056#1072#1079#1084#1077#1088' '#1092#1072#1081#1083#1086#1074#1086#1081' '#1089#1080#1089#1090#1077#1084#1077', '#1052#1041
  end
  object Label12: TLabel
    Left = 24
    Top = 197
    Width = 181
    Height = 13
    Caption = #1048#1089#1082#1072#1090#1100' '#1092#1072#1081#1083#1099' '#1086#1087#1088#1077#1076#1077#1083#1077#1085#1085#1086#1075#1086' '#1090#1080#1087#1072
  end
  object LabelTypeFile: TLabel
    Left = 22
    Top = 225
    Width = 265
    Height = 13
    Caption = #1054#1090#1084#1077#1090#1100#1077' '#1090#1080#1087#1099' '#1092#1072#1081#1083#1086#1074' '#1080#1079' '#1087#1088#1077#1076#1083#1086#1078#1077#1085#1085#1086#1075#1086' '#1095#1077#1082'-'#1083#1080#1089#1090#1072
    Enabled = False
  end
  object ButtonSelectVolume: TButton
    Left = 224
    Top = 16
    Width = 75
    Height = 25
    Caption = #1042#1099#1073#1088#1072#1090#1100'..'
    TabOrder = 0
    OnClick = ButtonSelectVolumeClick
  end
  object EditBD: TEdit
    Left = 328
    Top = 56
    Width = 299
    Height = 21
    TabOrder = 1
    Text = #1055#1091#1090#1100' '#1082' '#1041#1044
  end
  object ButtonSelectBD: TButton
    Left = 224
    Top = 54
    Width = 75
    Height = 25
    Caption = #1042#1099#1073#1088#1072#1090#1100'..'
    TabOrder = 2
    OnClick = ButtonSelectBDClick
  end
  object EditNameVolume: TEdit
    Left = 328
    Top = 18
    Width = 73
    Height = 21
    TabOrder = 3
    Text = #1052#1077#1090#1082#1072' '#1090#1086#1084#1072
  end
  object ButtonScan: TButton
    Left = 22
    Top = 352
    Width = 130
    Height = 33
    Caption = #1053#1072#1095#1072#1090#1100' '#1089#1082#1072#1085#1080#1088#1086#1074#1072#1085#1080#1077
    TabOrder = 4
    OnClick = ButtonScanClick
  end
  object ToggleSwitchSearchType: TToggleSwitch
    Left = 237
    Top = 199
    Width = 50
    Height = 20
    DisabledColor = clSilver
    FrameColor = clMoneyGreen
    ShowStateCaption = False
    TabOrder = 5
    ThumbColor = clRed
    OnClick = ToggleSwitchSearchTypeClick
  end
  object CheckListBoxType: TCheckListBox
    Left = 72
    Top = 244
    Width = 165
    Height = 69
    OnClickCheck = CheckListBoxTypeClickCheck
    Color = clMenu
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = 16
    Font.Name = 'Tahoma'
    Font.Style = []
    HeaderBackgroundColor = clGrayText
    Items.Strings = (
      'PNG'
      'JPEG'
      'BMP'
      'PDF')
    ParentFont = False
    TabOrder = 6
  end
  object VirtualStringTree1: TVirtualStringTree
    Left = 328
    Top = 85
    Width = 297
    Height = 321
    Colors.BorderColor = clWhite
    Header.AutoSizeIndex = 0
    Header.Font.Charset = DEFAULT_CHARSET
    Header.Font.Color = clWindowText
    Header.Font.Height = -11
    Header.Font.Name = 'Tahoma'
    Header.Font.Style = []
    Header.Height = 20
    Header.Options = [hoColumnResize, hoDrag, hoShowSortGlyphs, hoVisible]
    TabOrder = 7
    TreeOptions.AutoOptions = [toAutoDropExpand, toAutoScroll, toAutoScrollOnExpand, toAutoSort, toAutoTristateTracking, toAutoDeleteMovedNodes, toAutoChangeScale]
    TreeOptions.PaintOptions = [toShowButtons, toShowDropmark, toShowHorzGridLines, toShowRoot, toShowTreeLines, toShowVertGridLines, toThemeAware, toUseBlendedImages]
    TreeOptions.SelectionOptions = [toFullRowSelect]
    OnGetText = VirtualStringTree1GetText
    Touch.InteractiveGestures = [igPan, igPressAndTap]
    Touch.InteractiveGestureOptions = [igoPanSingleFingerHorizontal, igoPanSingleFingerVertical, igoPanInertia, igoPanGutter, igoParentPassthrough]
    Columns = <
      item
        Position = 0
        Width = 71
        WideText = 'ID'
      end
      item
        Position = 1
        Width = 103
        WideText = #1053#1086#1084#1077#1088' '#1050#1083#1072#1089#1090#1077#1088#1072
      end
      item
        Position = 2
        Width = 117
        WideText = #1056#1072#1089#1096#1080#1088#1077#1085#1080#1077' '#1092#1072#1081#1083#1072
      end>
  end
  object ButtonScanStop: TButton
    Left = 170
    Top = 352
    Width = 144
    Height = 33
    Caption = #1054#1089#1090#1072#1085#1086#1074#1080#1090#1100' '#1089#1082#1072#1085#1080#1088#1086#1074#1072#1085#1080#1077
    TabOrder = 8
    OnClick = ButtonScanStopClick
  end
  object OpenTextFileDialog1: TOpenTextFileDialog
    Filter = 
      #1060#1072#1081#1083#1099' SQLite '#1073#1072#1079' '#1076#1072#1085#1085#1099#1093' (*.db *.sqlite *.sqlite3 *.db3)|*.db;*.s' +
      'qlite;*.sqlite3;*.db3'
    Title = #1042#1099#1073#1077#1088#1080#1090#1077' '#1087#1091#1090#1100' '#1076#1086' '#1073#1072#1079#1099' '#1076#1072#1085#1085#1099#1093
    Left = 488
    Top = 8
  end
end
