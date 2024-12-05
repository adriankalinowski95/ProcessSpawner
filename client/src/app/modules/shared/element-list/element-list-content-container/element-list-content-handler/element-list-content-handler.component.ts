import {Component, EventEmitter, Input, Output} from '@angular/core';
import {ContentHandlerConfig} from "../../models/ElementListConfig";
import {shared} from "../../../shared";
import {HandelContentEvent} from "../../models/HandelContentEvent";
import ActionType = shared.action_button.models.ActionType;

@Component({
  selector: 'app-element-list-content-handler',
  templateUrl: './element-list-content-handler.component.html',
  styleUrl: './element-list-content-handler.component.scss'
})
export class ElementListContentHandlerComponent {
  @Input() contentHandlerConfig: ContentHandlerConfig;
  @Output() handelContent: EventEmitter<ActionType> = new EventEmitter<ActionType>();
  onActionButtonClick(event: ActionType) {
    this.handelContent.emit(event);
  }
}
