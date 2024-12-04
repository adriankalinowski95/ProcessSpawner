import { Component } from '@angular/core';
import { MatButtonModule } from '@angular/material/button';
import { MatSidenavModule } from '@angular/material/sidenav';
import { AuthService } from '../../../auth/services/auth.service';
import { Menu } from '../sidenav/sidenav.component';

@Component({
  selector: 'app-layout',
  templateUrl: './layout.component.html',
  styleUrl: './layout.component.scss',
})
export class LayoutComponent {
    isExpanded: boolean = false;

    isExpandedHandler(isExpanded: boolean) {
        this.isExpanded = isExpanded;
    }

    menu: Menu = [
        {
          title: 'Home',
          icon: 'home',
          link: '/home',
          color: '#ff7f0e',
          subMenu: [],
        },
        {
          title: 'Statistics',
          icon: 'bar_chart',
          color: '#ff7f0e',
          subMenu: [
            {
              title: 'Sales',
              icon: 'money',
              link: '/sales',
              color: '#ff7f0e',
              subMenu: [],
            },
            {
              title: 'Customers',
              icon: 'people',
              color: '#ff7f0e',
              link: '/customers',
                subMenu: [],
            },
          ],
        },
      ];
}