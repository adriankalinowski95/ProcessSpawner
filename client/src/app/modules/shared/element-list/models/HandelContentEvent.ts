import {shared} from "../../shared";
import ActionType = shared.action_button.models.ActionType;
import {DataSource} from "./data-source";
import {PageConfig} from "./pageConfig";

export interface HandelContentEvent {
  actionType: ActionType,
  content: DataSource[],
  pageConfig?: PageConfig;
}
