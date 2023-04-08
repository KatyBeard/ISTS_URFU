object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 442
  ClientWidth = 628
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  OldCreateOrder = True
  PixelsPerInch = 96
  TextHeight = 15
  object Edit1: TEdit
    Left = 144
    Top = 160
    Width = 257
    Height = 23
    TabOrder = 0
    Text = 'Group'
  end
  object Edit2: TEdit
    Left = 144
    Top = 208
    Width = 257
    Height = 23
    TabOrder = 1
    Text = 'FIO'
  end
  object Button1: TButton
    Left = 168
    Top = 96
    Width = 201
    Height = 42
    Caption = 'Output group and FIO'
    TabOrder = 2
    OnClick = Button1Click
  end
end
