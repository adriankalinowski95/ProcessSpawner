import {Component, EventEmitter, Input, Output} from '@angular/core';
import {ActionHandlerType, ActionPanelConfig} from "../models/dynamic-form-config";
import {shared} from "../../shared";
import Config = shared.action_button.models.Config;
import {ActionType} from "../../action-button/models/action-button-config";
import Type = shared.action_button.models.Type;

@Component({
  selector: 'app-dynamic-form-action-panel',
  templateUrl: './dynamic-form-action-panel.component.html',
  styleUrl: './dynamic-form-action-panel.component.scss'
})
export class DynamicFormActionPanelComponent {
  @Input() config: ActionPanelConfig;
  @Output() buttonClick: EventEmitter<ActionType> = new EventEmitter<ActionType>();

  actionHandlerType = ActionHandlerType;
  buttonConfig: Config;

  ngOnInit() {
    this.createButtonConfig()
  }

  onActionButtonClick(event: ActionType) {
    this.buttonClick.emit(event);
  }

  private createButtonConfig() {
    switch(this.config.actionHandlerType) {
      case this.actionHandlerType.Default:
        this.buttonConfig = {
          disabled: false,
          type: Type.Default,
          text: 'Submit',
          actionType: ActionType.Submit
        }
        break;
    }
  }


}
