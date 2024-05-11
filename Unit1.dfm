object Form1: TForm1
  Left = 0
  Top = 0
  Caption = #1057#1090#1072#1090#1080#1089#1090#1080#1095#1085#1072' '#1086#1073#1088#1086#1073#1082#1072' '
  ClientHeight = 583
  ClientWidth = 702
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poDesigned
  OnClick = RadioButtonUncertaintyClick
  TextHeight = 15
  object Label1: TLabel
    Left = 24
    Top = 16
    Width = 127
    Height = 15
    Caption = #1054#1073#1077#1088#1110#1090#1100' '#1088#1077#1078#1080#1084' '#1088#1086#1073#1086#1090#1080
  end
  object LabelDataFile: TLabel
    Left = 184
    Top = 176
    Width = 49
    Height = 15
    Caption = #1054#1073#1088#1072#1085#1086': '
  end
  object LabelQuantilesFile: TLabel
    Left = 184
    Top = 220
    Width = 49
    Height = 15
    Caption = #1054#1073#1088#1072#1085#1086': '
  end
  object Label2: TLabel
    Left = 468
    Top = 16
    Width = 126
    Height = 15
    Caption = #1056#1077#1079#1091#1083#1100#1090#1072#1090#1080' '#1088#1086#1079#1088#1072#1093#1091#1085#1082#1091
  end
  object RadioButtonError: TRadioButton
    Left = 24
    Top = 56
    Width = 139
    Height = 17
    Caption = #1056#1086#1079#1088#1072#1093#1091#1085#1086#1082' '#1087#1086#1093#1080#1073#1082#1080
    TabOrder = 0
    OnClick = RadioButtonErrorClick
  end
  object RadioButtonUncertainty: TRadioButton
    Left = 24
    Top = 96
    Width = 177
    Height = 17
    Caption = #1056#1086#1079#1088#1072#1093#1091#1085#1086#1082' '#1085#1077#1074#1080#1079#1085#1072#1095#1077#1085#1086#1089#1090#1110' '
    TabOrder = 1
    OnClick = RadioButtonUncertaintyClick
  end
  object ButtonLoadData: TButton
    Left = 28
    Top = 168
    Width = 135
    Height = 33
    Caption = #1047#1072#1074#1072#1085#1090#1072#1078#1080#1090#1080' '#1076#1072#1085#1110
    TabOrder = 2
    OnClick = ButtonLoadDataClick
  end
  object ButtonLoadQuantiles: TButton
    Left = 25
    Top = 212
    Width = 138
    Height = 33
    Caption = #1047#1072#1074#1072#1085#1090#1072#1078#1080#1090#1080' '#1082#1074#1072#1085#1090#1080#1083#1110
    TabOrder = 3
    OnClick = ButtonLoadQuantilesClick
  end
  object MemoResults: TMemo
    Left = 408
    Top = 53
    Width = 281
    Height = 177
    Lines.Strings = (
      'MemoResults')
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 4
  end
  object ButtonCalculate: TButton
    Left = 504
    Top = 248
    Width = 75
    Height = 25
    Caption = #1056#1086#1079#1088#1072#1093#1091#1074#1072#1090#1080
    TabOrder = 5
    OnClick = ButtonCalculateClick
  end
  object ChartError: TChart
    Left = 24
    Top = 288
    Width = 663
    Height = 282
    Title.Text.Strings = (
      'TChart')
    TabOrder = 6
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
    object BarSeriesError: TBarSeries
      HoverElement = []
      Title = 'BarSeriesError'
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Bar'
      YValues.Order = loNone
    end
    object Series1: TFastLineSeries
      HoverElement = []
      LinePen.Color = 3513587
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
  end
  object OpenDialogData: TOpenDialog
    Filter = 'Text and CSV Files|*.txt;*.csv'
    Left = 280
    Top = 168
  end
  object OpenDialogQuantiles: TOpenDialog
    Filter = 'Text and CSV Files|*.txt;*.csv'
    Left = 280
    Top = 216
  end
end
