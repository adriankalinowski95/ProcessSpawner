import { BreakpointObserver } from '@angular/cdk/layout';
import { ChangeDetectionStrategy, Component, Input, OnInit, ViewChild } from '@angular/core';
import { MatSidenav } from '@angular/material/sidenav';
  
@Component({
  selector: 'app-sidenav-container',
  templateUrl: './sidenav-container.component.html',
  styleUrl: './sidenav-container.component.scss'
})
export class SidenavContainerComponent implements OnInit {
    @Input() 
    public isExpanded: boolean = false;

    ngOnInit() {

    }

    isOpened() {
        return true;
    }

    sideNavWidth() {
         return this.isExpanded ? '250px' : '64px';
    }
}
