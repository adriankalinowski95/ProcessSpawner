import {Component, Input} from '@angular/core';
import {FilterConfig} from "../models/ElementListConfig";

@Component({
  selector: 'app-element-list-filter-container',
  templateUrl: './element-list-filter-container.component.html',
  styleUrl: './element-list-filter-container.component.scss'
})
export class ElementListFilterContainerComponent {
  @Input() filterConfig: FilterConfig;

}
