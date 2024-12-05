import {shared} from "../../shared";
import ActionType = shared.action_button.models.ActionType;
import {DataSource} from "./data-source";

export interface HandelContentEvent {
  actionType: ActionType,
  content: DataSource[],
}
