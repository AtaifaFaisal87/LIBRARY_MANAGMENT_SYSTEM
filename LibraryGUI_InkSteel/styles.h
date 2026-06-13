#pragma once
#include <QString>

// ============================================================
//  INK & STEEL — Colour Palette  (matches screenshot theme)
//  Background:  Deep slate-navy    #1A2332
//  Surface:     Dark card          #1E2D3D
//  Surface2:    Lighter card       #243447
//  Primary:     Teal/Cyan accent   #0D9488
//  Accent:      Steel blue         #1E6B8A
//  Sidebar:     Deep blue-grey     #16212E
//  SidebarSel:  Teal highlight     #0D9488
//  Success:     Emerald green      #10B981
//  Warning:     Amber              #F59E0B
//  Danger:      Rose red           #EF4444
//  Text:        Off-white          #E2E8F0
//  Muted:       Steel grey         #64748B
//  Border:      Dark border        #2D4059
// ============================================================

static const QString APP_STYLE = R"(
/* ---- Global ---- */
QWidget {
    background-color: #1A2332;
    color: #E2E8F0;
    font-family: "Segoe UI", "Inter", "Arial", sans-serif;
    font-size: 13px;
}

/* ---- Main Window / Dialogs ---- */
QMainWindow, QDialog {
    background-color: #1A2332;
}

/* ---- Menu Bar ---- */
QMenuBar {
    background-color: #16212E;
    color: #E2E8F0;
    border-bottom: 1px solid #2D4059;
    font-size: 13px;
}
QMenuBar::item {
    background: transparent;
    padding: 6px 12px;
}
QMenuBar::item:selected {
    background-color: #0D9488;
    color: #FFFFFF;
    border-radius: 3px;
}
QMenu {
    background-color: #1E2D3D;
    border: 1px solid #2D4059;
    color: #E2E8F0;
}
QMenu::item:selected {
    background-color: #0D9488;
    color: #FFFFFF;
}

/* ---- Group Boxes ---- */
QGroupBox {
    background-color: #1E2D3D;
    border: 1px solid #2D4059;
    border-radius: 8px;
    margin-top: 18px;
    padding: 12px;
    font-weight: bold;
    color: #94A3B8;
}
QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top left;
    padding: 2px 10px;
    color: #0D9488;
    font-size: 13px;
    letter-spacing: 1px;
}

/* ---- Push Buttons ---- */
QPushButton {
    background-color: #0D9488;
    color: #FFFFFF;
    border: none;
    border-radius: 6px;
    padding: 8px 18px;
    font-weight: bold;
    font-size: 12px;
    min-width: 80px;
}
QPushButton:hover {
    background-color: #0F9F92;
}
QPushButton:pressed {
    background-color: #0A7A6F;
}
QPushButton:disabled {
    background-color: #2D4059;
    color: #64748B;
}
QPushButton#dangerBtn {
    background-color: #DC2626;
    color: #FFFFFF;
}
QPushButton#dangerBtn:hover {
    background-color: #EF4444;
}
QPushButton#successBtn {
    background-color: #059669;
    color: #FFFFFF;
}
QPushButton#successBtn:hover {
    background-color: #10B981;
}
QPushButton#warningBtn {
    background-color: #D97706;
    color: #FFFFFF;
}
QPushButton#warningBtn:hover {
    background-color: #F59E0B;
}
QPushButton#cyanBtn {
    background-color: #1E6B8A;
    color: #FFFFFF;
}
QPushButton#cyanBtn:hover {
    background-color: #2580A4;
}

/* ---- Line Edits ---- */
QLineEdit {
    background-color: #243447;
    border: 1px solid #2D4059;
    border-radius: 6px;
    padding: 7px 10px;
    color: #E2E8F0;
    selection-background-color: #0D9488;
    selection-color: #FFFFFF;
}
QLineEdit:focus {
    border: 2px solid #0D9488;
    background-color: #1E2D3D;
}

/* ---- Combo Boxes ---- */
QComboBox {
    background-color: #243447;
    border: 1px solid #2D4059;
    border-radius: 6px;
    padding: 6px 10px;
    color: #E2E8F0;
    min-width: 120px;
}
QComboBox:focus {
    border: 2px solid #0D9488;
}
QComboBox QAbstractItemView {
    background-color: #1E2D3D;
    border: 1px solid #2D4059;
    selection-background-color: #0D9488;
    selection-color: #FFFFFF;
    color: #E2E8F0;
}
QComboBox::drop-down {
    border: none;
    width: 24px;
}

/* ---- Spin Boxes ---- */
QSpinBox, QDoubleSpinBox {
    background-color: #243447;
    border: 1px solid #2D4059;
    border-radius: 6px;
    padding: 6px;
    color: #E2E8F0;
}
QSpinBox:focus, QDoubleSpinBox:focus {
    border: 2px solid #0D9488;
}

/* ---- Labels ---- */
QLabel {
    color: #E2E8F0;
}
QLabel#titleLabel {
    color: #FFFFFF;
    font-size: 22px;
    font-weight: bold;
    letter-spacing: 1px;
}
QLabel#subtitleLabel {
    color: #64748B;
    font-size: 13px;
}
QLabel#sectionLabel {
    color: #94A3B8;
    font-size: 11px;
    font-weight: bold;
    letter-spacing: 2px;
}
QLabel#statValue {
    color: #FFFFFF;
    font-size: 26px;
    font-weight: bold;
}
QLabel#statLabel {
    color: #94A3B8;
    font-size: 10px;
    letter-spacing: 2px;
}
QLabel#warnLabel {
    color: #F59E0B;
    font-weight: bold;
}
QLabel#errorLabel {
    color: #EF4444;
    font-weight: bold;
}

/* ---- Table Widget ---- */
QTableWidget {
    background-color: #1E2D3D;
    border: 1px solid #2D4059;
    border-radius: 6px;
    gridline-color: #2D4059;
    alternate-background-color: #243447;
    color: #E2E8F0;
}
QTableWidget::item {
    padding: 8px;
}
QTableWidget::item:selected {
    background-color: rgba(13, 148, 136, 0.3);
    color: #FFFFFF;
}
QHeaderView::section {
    background-color: #16212E;
    color: #94A3B8;
    padding: 10px 8px;
    font-weight: bold;
    font-size: 11px;
    letter-spacing: 1px;
    border: none;
    border-right: 1px solid #2D4059;
    border-bottom: 1px solid #2D4059;
}
QHeaderView::section:first {
    border-top-left-radius: 6px;
}
QHeaderView::section:last {
    border-top-right-radius: 6px;
    border-right: none;
}

/* ---- Tab Widget ---- */
QTabWidget::pane {
    background-color: #1A2332;
    border: 1px solid #2D4059;
    border-radius: 6px;
}
QTabBar::tab {
    background-color: #16212E;
    color: #64748B;
    padding: 10px 20px;
    border-top-left-radius: 6px;
    border-top-right-radius: 6px;
    margin-right: 2px;
    font-weight: bold;
    font-size: 12px;
}
QTabBar::tab:selected {
    background-color: #1E2D3D;
    color: #0D9488;
    border-bottom: 2px solid #0D9488;
}
QTabBar::tab:hover:!selected {
    background-color: #1E2D3D;
    color: #94A3B8;
}

/* ---- Scroll Bars ---- */
QScrollBar:vertical {
    background-color: #16212E;
    width: 8px;
    border-radius: 4px;
}
QScrollBar::handle:vertical {
    background-color: #2D4059;
    border-radius: 4px;
    min-height: 20px;
}
QScrollBar::handle:vertical:hover {
    background-color: #0D9488;
}
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }

QScrollBar:horizontal {
    background-color: #16212E;
    height: 8px;
    border-radius: 4px;
}
QScrollBar::handle:horizontal {
    background-color: #2D4059;
    border-radius: 4px;
    min-width: 20px;
}
QScrollBar::handle:horizontal:hover {
    background-color: #0D9488;
}
QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0; }

/* ---- List Widget ---- */
QListWidget {
    background-color: #1E2D3D;
    border: 1px solid #2D4059;
    border-radius: 6px;
    color: #E2E8F0;
}
QListWidget::item {
    padding: 10px;
    border-bottom: 1px solid #2D4059;
}
QListWidget::item:selected {
    background-color: rgba(13, 148, 136, 0.25);
    color: #0D9488;
}

/* ---- Message Boxes ---- */
QMessageBox {
    background-color: #1E2D3D;
    color: #E2E8F0;
}

/* ---- Separator ---- */
QFrame[frameShape="4"], QFrame[frameShape="5"] {
    color: #2D4059;
}

/* ---- Status Bar ---- */
QStatusBar {
    background-color: #16212E;
    color: #64748B;
    border-top: 1px solid #2D4059;
    font-size: 12px;
}

/* ---- Tool Tips ---- */
QToolTip {
    background-color: #243447;
    color: #E2E8F0;
    border: 1px solid #2D4059;
    border-radius: 4px;
    padding: 4px 8px;
}

/* ---- Progress Bar ---- */
QProgressBar {
    background-color: #243447;
    border: 1px solid #2D4059;
    border-radius: 4px;
    text-align: center;
    color: #E2E8F0;
}
QProgressBar::chunk {
    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                      stop:0 #0D9488, stop:1 #1E6B8A);
    border-radius: 4px;
}
)";

// Card widget stylesheet — dark card surface
static const QString CARD_STYLE = R"(
    background-color: #1E2D3D;
    border: 1px solid #2D4059;
    border-radius: 10px;
    padding: 16px;
)";

// Stat card styles: dark navy cards with coloured top accent border
static const QString STAT_CARD_STYLE_BLUE = R"(
    background-color: #1E2D3D;
    border: 1px solid #2D4059;
    border-top: 3px solid #0D9488;
    border-radius: 10px;
    padding: 16px;
)";
static const QString STAT_CARD_STYLE_CYAN = R"(
    background-color: #1E2D3D;
    border: 1px solid #2D4059;
    border-top: 3px solid #1E6B8A;
    border-radius: 10px;
    padding: 16px;
)";
static const QString STAT_CARD_STYLE_GREEN = R"(
    background-color: #1E2D3D;
    border: 1px solid #2D4059;
    border-top: 3px solid #10B981;
    border-radius: 10px;
    padding: 16px;
)";
static const QString STAT_CARD_STYLE_ROSE = R"(
    background-color: #1E2D3D;
    border: 1px solid #2D4059;
    border-top: 3px solid #EF4444;
    border-radius: 10px;
    padding: 16px;
)";
