import {Component, EventEmitter, Input, Output} from '@angular/core';
import { ContentConfig } from '../models/ElementListConfig';
import {HandelContentEvent} from "../models/HandelContentEvent";
import {shared} from "../../shared";
import ActionType = shared.action_button.models.ActionType;
import {DataSource} from "../models/data-source";
import {DisplayedColumns} from "../models/displayed-columns";

@Component({
  selector: 'app-element-list-content-container',
  templateUrl: './element-list-content-container.component.html',
  styleUrl: './element-list-content-container.component.scss'
})
export class ElementListContentContainerComponent {
  @Input() contentConfig: ContentConfig;
  @Input() selectedElements: DataSource[] = [];
  @Input() dataSource: DataSource[];
  @Input() displayedColumns: DisplayedColumns;
  @Output() handelContent: EventEmitter<HandelContentEvent> = new EventEmitter<HandelContentEvent>();

  onHandleContentByPanel($event: ActionType) {
    this.handelContent.emit({actionType: $event, content: this.selectedElements});
  }
  onHandleContentByElement($event: HandelContentEvent) {
    this.handelContent.emit($event);
  }
  onSelectionChange($event: DataSource[]) {
    this.selectedElements = $event
  }
}
