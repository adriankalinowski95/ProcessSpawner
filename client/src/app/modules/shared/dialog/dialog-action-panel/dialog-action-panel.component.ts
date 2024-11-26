import {Component, EventEmitter, Input, Output} from '@angular/core';
import {shared} from "../../shared";
import ActionType = shared.action_button.models.ActionType;
import Config = shared.action_button.models.Config;

@Component({
  selector: 'app-dialog-action-panel',
  templateUrl: './dialog-action-panel.component.html',
  styleUrl: './dialog-action-panel.component.scss'
})
export class DialogActionPanelComponent {
  @Input() buttons: Config[]
  @Output() actionButtonClicked: EventEmitter<ActionType> = new EventEmitter<ActionType>();

  onActionButtonClick($event: ActionType) {
    this.actionButtonClicked.emit($event)
  }
}
