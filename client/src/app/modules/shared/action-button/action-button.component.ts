import {ChangeDetectionStrategy, Component, EventEmitter, Input, Output} from '@angular/core';
import {ActionType, Config} from "./models/action-button-config";

@Component({
  selector: 'app-action-button',
  templateUrl: './action-button.component.html',
  styleUrl: './action-button.component.scss',
  changeDetection: ChangeDetectionStrategy.OnPush
})
export class ActionButtonComponent {
  @Input() config: Config
  @Output() actionButtonClicked: EventEmitter<ActionType> = new EventEmitter<ActionType>();

  click(actionType: ActionType) {
    this.actionButtonClicked.emit(actionType);
  }
}
