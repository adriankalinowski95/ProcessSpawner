import {Component, EventEmitter, Input, Output} from '@angular/core';
import {MainConfig} from "./models/ElementListConfig";
import {defaultColumns, defaultConfig, ELEMENT_DATA} from "./models/defaultConfig";
import {HandelContentEvent} from "./models/HandelContentEvent";
import {shared} from "../shared";
import DataSource = shared.element_list.models.DataSource;
import {DisplayedColumns} from "./models/displayed-columns";
import { PageConfig } from './models/pageConfig';

@Component({
  selector: 'app-element-list',
  templateUrl: './element-list.component.html',
  styleUrl: './element-list.component.scss'
})
export class ElementListComponent {
  @Input() config: MainConfig;
  @Input() dataSource: DataSource[];
  @Input() displayedColumns: DisplayedColumns;
  @Output() handelContent: EventEmitter<HandelContentEvent> = new EventEmitter<HandelContentEvent>();
  @Output() pageChanged = new EventEmitter<PageConfig>();
  
  onHandleContent($event: HandelContentEvent) {
    this.handelContent.emit($event);
  }

  onPageChange($event: PageConfig) {
    this.pageChanged.emit($event);
  }
}
