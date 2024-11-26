import {Component, EventEmitter, Input, OnInit, Output} from '@angular/core';
import { ContentConfig } from '../models/ElementListConfig';
import {HandelContentEvent} from "../models/HandelContentEvent";
import {shared} from "../../shared";
import ActionType = shared.action_button.models.ActionType;
import {DataSource} from "../models/data-source";
import {DisplayedColumns} from "../models/displayed-columns";
import { PageConfig } from '../models/pageConfig';

@Component({
  selector: 'app-element-list-content-container',
  templateUrl: './element-list-content-container.component.html',
  styleUrl: './element-list-content-container.component.scss'
})
export class ElementListContentContainerComponent implements OnInit{
  @Input() contentConfig: ContentConfig;
  @Input() selectedElements: DataSource[] = [];
  @Input() dataSource: DataSource[];
  @Input() displayedColumns: DisplayedColumns;
  @Output() handelContent: EventEmitter<HandelContentEvent> = new EventEmitter<HandelContentEvent>();
  @Output() pageChanged = new EventEmitter<PageConfig>();

  private pageConfig: PageConfig;

  ngOnInit() {
    this.pageConfig = this.contentConfig.paginationConfig.pageConfig;
  }

  onHandleContentByPanel($event: ActionType) {
    if(this.contentConfig.usePagination) {
      this.handelContent.emit({actionType: $event, content: this.selectedElements, pageConfig: this.pageConfig});
    } else {
      this.handelContent.emit({actionType: $event, content: this.selectedElements});
    }
  }
  onHandleContentByElement($event: HandelContentEvent) {
    if(this.contentConfig.usePagination) {
      this.handelContent.emit({...$event, pageConfig: this.pageConfig});
    } else {
      this.handelContent.emit($event);
    }
  }
  onSelectionChange($event: DataSource[]) {
    this.selectedElements = $event
  }

  onPageChange($event: PageConfig) {
    this.pageChanged.emit($event);
    this.pageConfig = $event;
  }
}
