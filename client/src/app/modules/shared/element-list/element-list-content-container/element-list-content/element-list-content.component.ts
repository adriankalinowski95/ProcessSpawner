import {
  AfterViewInit,
  Component,
  EventEmitter,
  Input, OnChanges,
  OnInit,
  Output,
  SimpleChanges,
  ViewChild
} from '@angular/core';
import {SelectionModel} from "@angular/cdk/collections";
import {MatTableDataSource} from "@angular/material/table";
import {RowTableIcon, TableConfig} from "../../models/ElementListConfig";
import {HandelContentEvent} from "../../models/HandelContentEvent";
import {DataSource} from "../../models/data-source";
import {DisplayedColumns} from "../../models/displayed-columns";
import {shared} from "../../../shared";
import {MatSort} from "@angular/material/sort";

@Component({
  selector: 'app-element-list-content',
  templateUrl: './element-list-content.component.html',
  styleUrl: './element-list-content.component.scss'
})
export class ElementListContentComponent implements OnInit, AfterViewInit, OnChanges {
  @Input() config: TableConfig;
  @Input() dataSource: DataSource[];
  @Input() displayedColumns: DisplayedColumns;
  @Input() selectedElements: DataSource[];

  @Output() selectionChange: EventEmitter<DataSource[]> = new EventEmitter<DataSource[]>();
  @Output() handelContent: EventEmitter<HandelContentEvent> = new EventEmitter<HandelContentEvent>();

  @ViewChild(MatSort) sort: MatSort;

  actionType = shared.action_button.models.ActionType
  selection: SelectionModel<DataSource>;
  tableDataSource: MatTableDataSource<DataSource>;

  ngOnChanges(changes: SimpleChanges) {
    if(!changes["dataSource"]?.isFirstChange()) {
      this.tableDataSource = new MatTableDataSource<DataSource>(this.dataSource);
    }
  }
  ngOnInit() {
    this.tableDataSource = new MatTableDataSource<DataSource>(this.dataSource);
    this.applyConfiguration();
  }
  ngAfterViewInit() {
    this.tableDataSource.sort = this.sort;
  }
  isAllSelected() {
    const numSelected = this.selection.selected.length;
    const numRows = this.tableDataSource.data.length;
    return numSelected === numRows;
  }

  toggleAllRows() {
    if (this.isAllSelected()) {
      this.selection.clear();
      this.selectionChange.emit(this.selection.selected);
      setTimeout(() => {
        this.tableDataSource.sort = this.sort;
      });
      return;
    }
    this.selection.select(...this.tableDataSource.data);
    setTimeout(() => {
      this.tableDataSource.sort = this.sort;
    });
    this.selectionChange.emit(this.selection.selected);
  }
  checkboxLabel(row?: DataSource): string {
    if (!row) {
      return `${this.isAllSelected() ? 'deselect' : 'select'} all`;
    }
    return `${this.selection.isSelected(row) ? 'deselect' : 'select'} row ${row.index + 1}`;
  }
  mapDisplayedColumnsToColumnName(): string[] {
    return this.displayedColumns.map(column => column.name);
  }
  handelElement(actionType: shared.action_button.models.ActionType, element: DataSource, isDblClick = false) {
    if(isDblClick) {
      this.config.dbClickEdit && this.handelContent.emit({actionType: actionType, content: [element]});
      return;
    }
    this.handelContent.emit({actionType: actionType, content: [element]});
  }
  toggleElement(row: DataSource) {
    this.selection.toggle(row);
    setTimeout(() => {
      this.tableDataSource.sort = this.sort;
    });
    this.selectionChange.emit(this.selection.selected);
  }
  private applyConfiguration() {
    this.multipleSelection();
    this.configureRowIcons()
  }
  private multipleSelection() {
    this.selection = new SelectionModel<DataSource>(this.config.multiselect, this.selectedElements);
    this.displayedColumns.unshift({name: 'select', displayName: ''})
  }

  private configureRowIcons() {
    const iconDisplayColumn: DisplayedColumns = this.config.rowTableIcons.map(icon => { return { name: icon.name, displayName: icon.displayName || '' }});
    this.displayedColumns = [...this.displayedColumns, ...iconDisplayColumn]
  }
}
